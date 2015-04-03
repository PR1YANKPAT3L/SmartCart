<?php
define('CURRENT_VERSION', '1.0');

define('DSN', 'mysql:host=localhost;dbname=smartcart_db');
define('USER', 'root');
define('PASS', 'root');

define('STATUS_OK', 200);
define('STATIC_CREATED', 201);
define('STATUS_ACCEPTED', 202);
define('STATUS_NO_CONTENT', 204);

define('STATUS_MULTIPLE_CHOICES', 300);
define('STATUS_MOVED_PERMANENTLY', 301);
define('STATUS_FOUND', 302);
define('STATUS_NOT_MODIFIED', 304);
define('STATUS_USE_PROXY', 305);
define('STATUS_TEMPORARY_REDIRECT', 307);

define('STATUS_BAD_REQUEST', 400);
define('STATUS_UNAUTHORIZED', 401);
define('STATUS_FORBIDDEN', 403);
define('STATUS_NOT_FOUND', 404);
define('STATUS_METHOD_NOT_ALLOWED', 405);
define('STATUS_NOT_ACCEPTED', 406);

define('STATUS_INTERNAL_SERVER_ERROR', 500);
define('STATUS_NOT_IMPLEMENTED', 501);