<?php
namespace Slim\Middleware;

class OAuth2Auth extends \Slim\Middleware {
	protected $headers = array();
	protected $config;
	protected $mysql;

	public function __construct($headers, $config, $mysql) {
		$this->headers = $headers;
		$this->config = $config;
		$this->mysql = $mysql;
	}

	public function call() {
		if(!isset($this->headers['Authorization'])) {
			$this->app->getLog()->error('No Authorization Headers Sent');
			$view = $this->app->view();
			$view->setData('data', array('message' => "Access Credentials Not Supplied"));
			$view->render('error.php', 400);
		} else {
			try {
				$authHeader = $this->headers['Authorization'];
				$auth = new \Service\Mysql\AuthService($this->mysql, $this->config);
				$validate_user_id = $auth->verifyOAuth($authHeader);
				$this->app->user_id = $validate_user_id;
			} catch (Exception $e) {
				$view = $this->app->view();
				$view->setData("data", array("message" => $e->getMessage()));
				$view->render('error.php', $e->getCode());
			}
		}

		// Proceed the application
		$this->next->call();
	}
}