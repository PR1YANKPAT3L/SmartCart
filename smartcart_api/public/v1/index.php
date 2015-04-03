<?php
// Define Global Variables
define('VENDOR_LOC', dirname(dirname(dirname(__FILE__))) . '/vendor');

// Load Required Files
require VENDOR_LOC . '/autoload.php';

// Register Slim Auto-Loader
\Slim\Slim::registerAutoLoader();

// Set up Database Connection
R::setup(DSN, USER, PASS);
R::freeze(true);

// Initialize App
$app = new \Slim\Slim(array(
	'cookies.secret.key' => md5('smartcart'),
));

// Handle GET Requests
\Slim\Route::setDefaultConditions(array(
	'id' => '[0-9]{1,}',
));

class ResourceNotFoundException extends Exception {}

/*$token = bin2hex(openssl_random_pseudo_bytes(16));
$headers = apache_request_headers();
$app->add(new \Slim\Middleware\OAuth2Auth($headers, '', new PDO(DSN, USER, PASS)));*/

//********************************************************************************
// Validates the oauthentication from the tokens provided
// Call this first to gain access to oauth
// METHOD: POST
// http://localhost/v1/oauth/caf88f88861fbbef682aa76b35a3ecca66674999/4bf15ddf19182211513647d87e40407b546cf5a2/457901444/1427260842/1.0
//********************************************************************************
$app->post('/oauth/:access_token/:refresh_token/:oauth_nonce/:oauth_timestamp/:oauth_version', function($access_token, $refresh_token, $oauth_nonce, $oauth_timestamp, $oauth_version) use ($app) {
	try {
		$old_oauth_noonce = 0;
		$oauth = "";
		if($oauth_version === CURRENT_VERSION) {
			if($old_oauth_noonce != $oauth_nonce) {
				$old_oauth_noonce = $oauth_nonce;
				$oauth = R::findOne('user_oauth_info', 'access_token=:access_token AND refresh_token=:refresh_token AND oauth_version=:oauth_version', array(":access_token" => $access_token, ":refresh_token" => $refresh_token, "oauth_version" => $oauth_version));
			}			
			if($oauth && $old_oauth_noonce == $oauth_nonce) {
				$app->response()->header('Content-Type', 'application/json');
				$app->response()->header('Refresh-Token', $refresh_token);
				echo json_encode(array(
					"success"=> array(
						"access_token" => $oauth['access_token'],
						"user_id" => $oauth['user_id']
					)
				));
			}
		} else {
			$app->response()->status(STATUS_BAD_REQUEST);
			$app->response()->header('X-Status-Reason', 'Version Mismatch or Outdated Version');
			$app->response()->header('Content-Type', 'application/json');
			echo json_encode(array(
				"errors" => array(
					"code" => 400,
					"message" => "version mismatch or outdated version"				
				)
			));
		}
	} catch(Exception $e) {
		$app->response()->status(STATUS_BAD_REQUEST);
		$app->response()->header('X-Status-Reason', $e->getMessage());
	}
});

//********************************************************************************
// Authenticate Login Information
// Call this first to gain access to API methods
// METHOD: POST
// http://localhost/v1/user/silentcoder/$2y$10$UZRf5BkOzfo2nIGtcfHWP.1szEyEUpHZ42ziIObum5j34kRedDIFC
//********************************************************************************
$app->post('/user/:username/:passwordhash', function ($username, $passwordhash) use ($app) {
	try {
		$result = R::findOne('sm_users', 'username=:username AND password=:password', array(":username" => $username, ":password" => $passwordhash));
		if($result) {
			$app->response()->header('Content-Type', 'application/json');
			$loginhistory = R::dispense("loginhistory");
			$loginhistory->accID = (string)$result['id'];
			$loginhistory->loginStatus = "Success";
			$loginhistory->lastLogin = time();
			$loginhistory->localRemoteAddr = $_SERVER['REMOTE_ADDR'];

			echo json_encode(array(
				"success" => array(
					"username" => $result['username'],
					"createOn" => $result['created_at'],
					"updatedOn" => $result['updated_at'],
					"logged" => R::store($loginhistory) ? true : false
				)
			));
		} else {
			$app->response()->header('Content-Type', 'application/json');
			echo json_encode(array(
				"errors" => array(
					"code" => 400,
					"message" => "invalid user credentials"
				)
			));
		}
	} catch(Exception $e) {
		$app->response()->status(STATUS_BAD_REQUEST);
		$app->response()->header('X-Status-Reason', $e->getMessage());
	}
});

//********************************************************************************
// Display list of products
// Call this first to gain access to API methods
// METHOD: GET
// http://localhost/v1/products
//********************************************************************************
$app->get('/products', function () use ($app) {    
	try {
		$products = R::exportAll(R::find('products'));
		$app->response()->header('Content-Type', 'application/json');
		if($products) {
			echo json_encode($products);
		} else {
			$app->response()->header('Content-Type', 'application/json');
			echo json_encode(array(
				"errors" => array(
					"code" => 400,
					"message" => "no products found"
				)
			));
		}
	} catch(Exception $e) {
		$app->response()->status(STATUS_BAD_REQUEST);
		$app->response()->header('X-Status-Reason', $e->getMessage());
	}
});

//********************************************************************************
// Display product details based on the id
// Call this first to gain access to API methods
// METHOD: GET
// http://localhost/v1/products/0
//********************************************************************************
$app->get('/products/:id', function ($id) use ($app) {    
	try {
		$products = R::findOne('products', 'id=?', array($id));
		$app->response()->header('Content-Type', 'application/json');
		if($products) {
			echo json_encode(array (
				"id" => $products['id'],
				"category" => $products['category'],
				"name" => $products['name'],
				"promotionalcode" => $products['promotionalcode'],
				"details" => $products['details'],
				"tag" => $products['tag']
			));
		} else {
			$app->response()->header('Content-Type', 'application/json');
			echo json_encode(array(
				"errors" => array(
					"code" => 400,
					"message" => "invalid product id"
				)
			));
		}
	} catch(Exception $e) {
		$app->response()->status(STATUS_BAD_REQUEST);
		$app->response()->header('X-Status-Reason', $e->getMessage());
	}
});


// Run the App
$app->run();