<?php

/**
 * @file
 * Drupal site-specific configuration file.
 *
 * IMPORTANT NOTE:
 * This file may have been set to read-only by the Drupal installation program.
 * If you make changes to this file, be sure to protect it again after making
 * your modifications. Failure to remove write permissions to this file is a
 * security risk.
 *
 * The configuration file to be loaded is based upon the rules below. However
 * if the multisite aliasing file named sites/sites.php is present, it will be
 * loaded, and the aliases in the array $sites will override the default
 * directory rules below. See sites/example.sites.php for more information about
 * aliases.
 *
 * The configuration directory will be discovered by stripping the website's
 * hostname from left to right and pathname from right to left. The first
 * configuration file found will be used and any others will be ignored. If no
 * other configuration file is found then the default configuration file at
 * 'sites/default' will be used.
 *
 * For example, for a fictitious site installed at
 * http://www.drupal.org:8080/mysite/test/, the 'settings.php' file is searched
 * for in the following directories:
 *
 * - sites/8080.www.drupal.org.mysite.test
 * - sites/www.drupal.org.mysite.test
 * - sites/drupal.org.mysite.test
 * - sites/org.mysite.test
 *
 * - sites/8080.www.drupal.org.mysite
 * - sites/www.drupal.org.mysite
 * - sites/drupal.org.mysite
 * - sites/org.mysite
 *
 * - sites/8080.www.drupal.org
 * - sites/www.drupal.org
 * - sites/drupal.org
 * - sites/org
 *
 * - sites/default
 *
 * Note that if you are installing on a non-standard port number, prefix the
 * hostname with that number. For example,
 * http://www.drupal.org:8080/mysite/test/ could be loaded from
 * sites/8080.www.drupal.org.mysite.test/.
 *
 * @see example.sites.php
 * @see conf_path()
 */

/**
 * Make sure DRUPAL_ROOT is always defined.
 */
if (!defined('DRUPAL_ROOT')) {
  define('DRUPAL_ROOT', '/var/www/drupal/sandbox');
}

/**
 * Database settings:
 *
 * The $databases array specifies the database connection or
 * connections that Drupal may use.  Drupal is able to connect
 * to multiple databases, including multiple types of databases,
 * during the same request.
 *
 * Each database connection is specified as an array of settings,
 * similar to the following:
 * @code
 * array(
 *   'driver' => 'mysql',
 *   'database' => 'databasename',
 *   'username' => 'username',
 *   'password' => 'password',
 *   'host' => 'localhost',
 *   'port' => 3306,
 *   'prefix' => 'myprefix_',
 *   'collation' => 'utf8_general_ci',
 * );
 * @endcode
 *
 * The "driver" property indicates what Drupal database driver the
 * connection should use.  This is usually the same as the name of the
 * database type, such as mysql or sqlite, but not always.  The other
 * properties will vary depending on the driver.  For SQLite, you must
 * specify a database file name in a directory that is writable by the
 * webserver.  For most other drivers, you must specify a
 * username, password, host, and database name.
 *
 * Some database engines support transactions.  In order to enable
 * transaction support for a given database, set the 'transactions' key
 * to TRUE.  To disable it, set it to FALSE.  Note that the default value
 * varies by driver.  For MySQL, the default is FALSE since MyISAM tables
 * do not support transactions.
 *
 * For each database, you may optionally specify multiple "target" databases.
 * A target database allows Drupal to try to send certain queries to a
 * different database if it can but fall back to the default connection if not.
 * That is useful for master/slave replication, as Drupal may try to connect
 * to a slave server when appropriate and if one is not available will simply
 * fall back to the single master server.
 *
 * The general format for the $databases array is as follows:
 * @code
 * $databases['default']['default'] = $info_array;
 * $databases['default']['slave'][] = $info_array;
 * $databases['default']['slave'][] = $info_array;
 * $databases['extra']['default'] = $info_array;
 * @endcode
 *
 * In the above example, $info_array is an array of settings described above.
 * The first line sets a "default" database that has one master database
 * (the second level default).  The second and third lines create an array
 * of potential slave databases.  Drupal will select one at random for a given
 * request as needed.  The fourth line creates a new database with a name of
 * "extra".
 *
 * For a single database configuration, the following is sufficient:
 * @code
 * $databases['default']['default'] = array(
 *   'driver' => 'mysql',
 *   'database' => 'databasename',
 *   'username' => 'username',
 *   'password' => 'password',
 *   'host' => 'localhost',
 *   'prefix' => 'main_',
 *   'collation' => 'utf8_general_ci',
 * );
 * @endcode
 *
 * You can optionally set prefixes for some or all database table names
 * by using the 'prefix' setting. If a prefix is specified, the table
 * name will be prepended with its value. Be sure to use valid database
 * characters only, usually alphanumeric and underscore. If no prefixes
 * are desired, leave it as an empty string ''.
 *
 * To have all database names prefixed, set 'prefix' as a string:
 * @code
 *   'prefix' => 'main_',
 * @endcode
 * To provide prefixes for specific tables, set 'prefix' as an array.
 * The array's keys are the table names and the values are the prefixes.
 * The 'default' element is mandatory and holds the prefix for any tables
 * not specified elsewhere in the array. Example:
 * @code
 *   'prefix' => array(
 *     'default'   => 'main_',
 *     'users'     => 'shared_',
 *     'sessions'  => 'shared_',
 *     'role'      => 'shared_',
 *     'authmap'   => 'shared_',
 *   ),
 * @endcode
 * You can also use a reference to a schema/database as a prefix. This may be
 * useful if your Drupal installation exists in a schema that is not the default
 * or you want to access several databases from the same code base at the same
 * time.
 * Example:
 * @code
 *   'prefix' => array(
 *     'default'   => 'main.',
 *     'users'     => 'shared.',
 *     'sessions'  => 'shared.',
 *     'role'      => 'shared.',
 *     'authmap'   => 'shared.',
 *   );
 * @endcode
 * NOTE: MySQL and SQLite's definition of a schema is a database.
 *
 * Advanced users can add or override initial commands to execute when
 * connecting to the database server, as well as PDO connection settings. For
 * example, to enable MySQL SELECT queries to exceed the max_join_size system
 * variable, and to reduce the database connection timeout to 5 seconds:
 *
 * @code
 * $databases['default']['default'] = array(
 *   'init_commands' => array(
 *     'big_selects' => 'SET SQL_BIG_SELECTS=1',
 *   ),
 *   'pdo' => array(
 *     PDO::ATTR_TIMEOUT => 5,
 *   ),
 * );
 * @endcode
 *
 * WARNING: These defaults are designed for database portability. Changing them
 * may cause unexpected behavior, including potential data loss.
 *
 * @see DatabaseConnection_mysql::__construct
 * @see DatabaseConnection_pgsql::__construct
 * @see DatabaseConnection_sqlite::__construct
 *
 * Database configuration format:
 * @code
 *   $databases['default']['default'] = array(
 *     'driver' => 'mysql',
 *     'database' => 'databasename',
 *     'username' => 'username',
 *     'password' => 'password',
 *     'host' => 'localhost',
 *     'prefix' => '',
 *   );
 *   $databases['default']['default'] = array(
 *     'driver' => 'pgsql',
 *     'database' => 'databasename',
 *     'username' => 'username',
 *     'password' => 'password',
 *     'host' => 'localhost',
 *     'prefix' => '',
 *   );
 *   $databases['default']['default'] = array(
 *     'driver' => 'sqlite',
 *     'database' => '/path/to/databasefilename',
 *   );
 * @endcode
 */
if (defined('DO_NOT_INCLUDE_PASSWORDS')) {
  $dbp = array(
    'www_user' => NULL,
    'www_user_new' => NULL,
    'wwwdev_user' => NULL,
    'wwwdev2_user' => NULL,
    'sandbox_user' => NULL,
  );
  $dhs = NULL;
}
else {
  include_once DRUPAL_ROOT . '/sites/default/passwords.php';
}

$databases = array (
  'file_db' =>
  array (
    'default' =>
    array (
      'database' => 'web_files',
      'username' => 'www_user',
      'password' => $dbp['www_user'],
      'host' => '192.251.101.224',
      'port' => '5095',
      'driver' => 'pgsql',
      'prefix' => '',
      'transactions' => TRUE,
      'sslmode' => 'disable', # disable, allow, prefer, require (prefer disable when only providing public access)
      'connect_timeout' => 2,
    ),
  ),
  'wwwdev' =>
  array (
    'default' =>
    array (
      'database' => 'wwwdev',
      'username' => 'wwwdev_user',
      'password' => $dbp['wwwdev_user'],
      'host' => '192.251.101.223',
      'port' => '5095',
      'driver' => 'pgsql',
      'prefix' => '',
      'transactions' => TRUE,
      'connect_timeout' => 2,
    ),
  ),
  'wwwdev2' =>
  array (
    'default' =>
    array (
      'database' => 'wwwdev2',
      'username' => 'www_user',
      'password' => $dbp['wwwdev2_user'],
      'host' => '192.251.101.223',
      'port' => '5095',
      'driver' => 'pgsql',
      'prefix' => '',
      'transactions' => TRUE,
      'connect_timeout' => 2,
    ),
  ),
  'sandbox' =>
  array (
    'default' =>
    array (
      'database' => 'sandbox',
      'username' => 'sandbox_user',
      'password' => $dbp['sandbox_user'],
      'host' => '192.251.101.223',
      'port' => '5095',
      'driver' => 'pgsql',
      'prefix' => '',
      'transactions' => TRUE,
      'connect_timeout' => 2,
    ),
  ),
  'www' =>
  array (
    'default' =>
    array (
      'database' => 'www',
      'username' => 'www_user',
      'password' => $dbp['www_user'],
      'host' => '192.251.101.224',
      'port' => '5095',
      'driver' => 'pgsql',
      'prefix' => '',
      'transactions' => TRUE,
      'connect_timeout' => 2,
    ),
  ),
);

$databases['default'] = $databases['sandbox'];
unset($dbp);

/**
 * Access control for update.php script.
 *
 * If you are updating your Drupal installation using the update.php script but
 * are not logged in using either an account with the "Administer software
 * updates" permission or the site maintenance account (the account that was
 * created during installation), you will need to modify the access check
 * statement below. Change the FALSE to a TRUE to disable the access check.
 * After finishing the upgrade, be sure to open this file again and change the
 * TRUE back to a FALSE!
 */
$update_free_access = FALSE;

/**
 * Salt for one-time login links and cancel links, form tokens, etc.
 *
 * This variable will be set to a random value by the installer. All one-time
 * login links will be invalidated if the value is changed. Note that if your
 * site is deployed on a cluster of web servers, you must ensure that this
 * variable has the same value on each server. If this variable is empty, a hash
 * of the serialized database credentials will be used as a fallback salt.
 *
 * For enhanced security, you may set this variable to a value using the
 * contents of a file outside your docroot that is never saved together
 * with any backups of your Drupal files and database.
 *
 * Example:
 *   $drupal_hash_salt = file_get_contents('/home/example/salt.txt');
 *
 */
$drupal_hash_salt = $dhs;
unset($dhs);

/**
 * Base URL (optional).
 *
 * If Drupal is generating incorrect URLs on your site, which could
 * be in HTML headers (links to CSS and JS files) or visible links on pages
 * (such as in menus), uncomment the Base URL statement below (remove the
 * leading hash sign) and fill in the absolute URL to your Drupal installation.
 *
 * You might also want to force users to use a given domain.
 * See the .htaccess file for more information.
 *
 * Examples:
 *   $base_url = 'http://www.example.com';
 *   $base_url = 'http://www.example.com:8888';
 *   $base_url = 'http://www.example.com/drupal';
 *   $base_url = 'https://www.example.com:8888/drupal';
 *
 * It is not allowed to have a trailing slash; Drupal will add it
 * for you.
 */
if (isset($_SERVER['SERVER_PORT']) && $_SERVER['SERVER_PORT'] == 443) {
  $base_url = 'https://sandbox.mcneese.edu';  // NO trailing slash!
}
else {
  $base_url = 'http://sandbox.mcneese.edu';  // NO trailing slash!
}

/**
 * PHP settings:
 *
 * To see what PHP settings are possible, including whether they can be set at
 * runtime (by using ini_set()), read the PHP documentation:
 * http://www.php.net/manual/en/ini.list.php
 * See drupal_environment_initialize() in includes/bootstrap.inc for required
 * runtime settings and the .htaccess file for non-runtime settings. Settings
 * defined there should not be duplicated here so as to avoid conflict issues.
 */

/**
 * Some distributions of Linux (most notably Debian) ship their PHP
 * installations with garbage collection (gc) disabled. Since Drupal depends on
 * PHP's garbage collection for clearing sessions, ensure that garbage
 * collection occurs by using the most common settings.
 */
ini_set('session.gc_probability', 1);
ini_set('session.gc_divisor', 1000);

/**
 * Set session lifetime (in seconds), i.e. the time from the user's last visit
 * to the active session may be deleted by the session garbage collector. When
 * a session is deleted, authenticated users are logged out, and the contents
 * of the user's $_SESSION variable is discarded.
 */
ini_set('session.gc_maxlifetime', 259200); // 3 days

/**
 * Set session cookie lifetime (in seconds), i.e. the time from the session is
 * created to the cookie expires, i.e. when the browser is expected to discard
 * the cookie. The value 0 means "until the browser is closed".
 */
ini_set('session.cookie_lifetime', 2764800); // 32 days

/**
 * If you encounter a situation where users post a large amount of text, and
 * the result is stripped out upon viewing but can still be edited, Drupal's
 * output filter may not have sufficient memory to process it.  If you
 * experience this issue, you may wish to uncomment the following two lines
 * and increase the limits of these variables.  For more information, see
 * http://php.net/manual/en/pcre.configuration.php.
 */
# ini_set('pcre.backtrack_limit', 200000);
# ini_set('pcre.recursion_limit', 200000);

/**
 * Drupal automatically generates a unique session cookie name for each site
 * based on its full domain name. If you have multiple domains pointing at the
 * same Drupal site, you can either redirect them all to a single domain (see
 * comment in .htaccess), or uncomment the line below and specify their shared
 * base domain. Doing so assures that users remain logged in as they cross
 * between your various domains. Make sure to always start the $cookie_domain
 * with a leading dot, as per RFC 2109.
 */
$cookie_domain = '.sandbox.mcneese.edu';

/**
 * Variable overrides:
 *
 * To override specific entries in the 'variable' table for this site,
 * set them here. You usually don't need to use this feature. This is
 * useful in a configuration file for a vhost or directory, rather than
 * the default settings.php. Any configuration setting from the 'variable'
 * table can be given a new value. Note that any values you provide in
 * these variable overrides will not be modifiable from the Drupal
 * administration interface.
 *
 * The following overrides are examples:
 * - site_name: Defines the site's name.
 * - theme_default: Defines the default theme for this site.
 * - anonymous: Defines the human-readable name of anonymous users.
 * Remove the leading hash signs to enable.
 */
$conf['site_name'] = 'McNeese State University';
$conf['theme_default'] = 'mcneese_www';
# $conf['anonymous'] = 'Visitor';

/**
 * A custom theme can be set for the offline page. This applies when the site
 * is explicitly set to maintenance mode through the administration page or when
 * the database is inactive due to an error. It can be set through the
 * 'maintenance_theme' key. The template file should also be copied into the
 * theme. It is located inside 'modules/system/maintenance-page.tpl.php'.
 * Note: This setting does not apply to installation and update pages.
 */
$conf['maintenance_theme'] = 'mcneese_www';

/**
 * Reverse Proxy Configuration:
 *
 * Reverse proxy servers are often used to enhance the performance
 * of heavily visited sites and may also provide other site caching,
 * security, or encryption benefits. In an environment where Drupal
 * is behind a reverse proxy, the real IP address of the client should
 * be determined such that the correct client IP address is available
 * to Drupal's logging, statistics, and access management systems. In
 * the most simple scenario, the proxy server will add an
 * X-Forwarded-For header to the request that contains the client IP
 * address. However, HTTP headers are vulnerable to spoofing, where a
 * malicious client could bypass restrictions by setting the
 * X-Forwarded-For header directly. Therefore, Drupal's proxy
 * configuration requires the IP addresses of all remote proxies to be
 * specified in $conf['reverse_proxy_addresses'] to work correctly.
 *
 * Enable this setting to get Drupal to determine the client IP from
 * the X-Forwarded-For header (or $conf['reverse_proxy_header'] if set).
 * If you are unsure about this setting, do not have a reverse proxy,
 * or Drupal operates in a shared hosting environment, this setting
 * should remain commented out.
 *
 * In order for this setting to be used you must specify every possible
 * reverse proxy IP address in $conf['reverse_proxy_addresses'].
 * If a complete list of reverse proxies is not available in your
 * environment (for example, if you use a CDN) you may set the
 * $_SERVER['REMOTE_ADDR'] variable directly in settings.php.
 * Be aware, however, that it is likely that this would allow IP
 * address spoofing unless more advanced precautions are taken.
 */
# $conf['reverse_proxy'] = TRUE;

/**
 * Specify every reverse proxy IP address in your environment.
 * This setting is required if $conf['reverse_proxy'] is TRUE.
 */
# $conf['reverse_proxy_addresses'] = array('a.b.c.d', ...);

/**
 * Set this value if your proxy server sends the client IP in a header
 * other than X-Forwarded-For.
 */
# $conf['reverse_proxy_header'] = 'HTTP_X_CLUSTER_CLIENT_IP';

/**
 * Page caching:
 *
 * By default, Drupal sends a "Vary: Cookie" HTTP header for anonymous page
 * views. This tells a HTTP proxy that it may return a page from its local
 * cache without contacting the web server, if the user sends the same Cookie
 * header as the user who originally requested the cached page. Without "Vary:
 * Cookie", authenticated users would also be served the anonymous page from
 * the cache. If the site has mostly anonymous users except a few known
 * editors/administrators, the Vary header can be omitted. This allows for
 * better caching in HTTP proxies (including reverse proxies), i.e. even if
 * clients send different cookies, they still get content served from the cache.
 * However, authenticated users should access the site directly (i.e. not use an
 * HTTP proxy, and bypass the reverse proxy if one is used) in order to avoid
 * getting cached pages from the proxy.
 */
# $conf['omit_vary_cookie'] = TRUE;

/**
 * CSS/JS aggregated file gzip compression:
 *
 * By default, when CSS or JS aggregation and clean URLs are enabled Drupal will
 * store a gzip compressed (.gz) copy of the aggregated files. If this file is
 * available then rewrite rules in the default .htaccess file will serve these
 * files to browsers that accept gzip encoded content. This allows pages to load
 * faster for these users and has minimal impact on server load. If you are
 * using a webserver other than Apache httpd, or a caching reverse proxy that is
 * configured to cache and compress these files itself you may want to uncomment
 * one or both of the below lines, which will prevent gzip files being stored.
 */
$conf['css_gzip_compression'] = FALSE;
$conf['js_gzip_compression'] = FALSE;

/**
 * String overrides:
 *
 * To override specific strings on your site with or without enabling the Locale
 * module, add an entry to this list. This functionality allows you to change
 * a small number of your site's default English language interface strings.
 *
 * Remove the leading hash signs to enable.
 */
# $conf['locale_custom_strings_en'][''] = array(
#   'forum'      => 'Discussion board',
#   '@count min' => '@count minutes',
# );

/**
 *
 * IP blocking:
 *
 * To bypass database queries for denied IP addresses, use this setting.
 * Drupal queries the {blocked_ips} table by default on every page request
 * for both authenticated and anonymous users. This allows the system to
 * block IP addresses from within the administrative interface and before any
 * modules are loaded. However on high traffic websites you may want to avoid
 * this query, allowing you to bypass database access altogether for anonymous
 * users under certain caching configurations.
 *
 * If using this setting, you will need to add back any IP addresses which
 * you may have blocked via the administrative interface. Each element of this
 * array represents a blocked IP address. Uncommenting the array and leaving it
 * empty will have the effect of disabling IP blocking on your site.
 *
 * Remove the leading hash signs to enable.
 */
# $conf['blocked_ips'] = array(
#   'a.b.c.d',
# );

/**
 * Fast 404 pages:
 *
 * Drupal can generate fully themed 404 pages. However, some of these responses
 * are for images or other resource files that are not displayed to the user.
 * This can waste bandwidth, and also generate server load.
 *
 * The options below return a simple, fast 404 page for URLs matching a
 * specific pattern:
 * - 404_fast_paths_exclude: A regular expression to match paths to exclude,
 *   such as images generated by image styles, or dynamically-resized images.
 *   If you need to add more paths, you can add '|path' to the expression.
 * - 404_fast_paths: A regular expression to match paths that should return a
 *   simple 404 page, rather than the fully themed 404 page. If you don't have
 *   any aliases ending in htm or html you can add '|s?html?' to the expression.
 * - 404_fast_html: The html to return for simple 404 pages.
 *
 * Add leading hash signs if you would like to disable this functionality.
 */
#$conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';
#$conf['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
#$conf['404_fast_html'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

/**
 * By default the page request process will return a fast 404 page for missing
 * files if they match the regular expression set in '404_fast_paths' and not
 * '404_fast_paths_exclude' above. 404 errors will simultaneously be logged in
 * the Drupal system log.
 *
 * You can choose to return a fast 404 page earlier for missing pages (as soon
 * as settings.php is loaded) by uncommenting the line below. This speeds up
 * server response time when loading 404 error pages and prevents the 404 error
 * from being logged in the Drupal system log. In order to prevent valid pages
 * such as image styles and other generated content that may match the
 * '404_fast_html' regular expression from returning 404 errors, it is necessary
 * to add them to the '404_fast_paths_exclude' regular expression above. Make
 * sure that you understand the effects of this feature before uncommenting the
 * line below.
 */
# drupal_fast_404();

/**
 * see: http://drupal.org/drupal-7.20-release-notes
 */
#$conf['image_allow_insecure_derivatives'] = TRUE;
$conf['image_allow_insecure_derivatives'] = FALSE;

/**
 * External access proxy settings:
 *
 * If your site must access the Internet via a web proxy then you can enter
 * the proxy settings here. Currently only basic authentication is supported
 * by using the username and password variables. The proxy_user_agent variable
 * can be set to NULL for proxies that require no User-Agent header or to a
 * non-empty string for proxies that limit requests to a specific agent. The
 * proxy_exceptions variable is an array of host names to be accessed directly,
 * not via proxy.
 */
# $conf['proxy_server'] = '';
# $conf['proxy_port'] = 8080;
# $conf['proxy_username'] = '';
# $conf['proxy_password'] = '';
# $conf['proxy_user_agent'] = '';
# $conf['proxy_exceptions'] = array('127.0.0.1', 'localhost');

/**
 * Authorized file system operations:
 *
 * The Update manager module included with Drupal provides a mechanism for
 * site administrators to securely install missing updates for the site
 * directly through the web user interface. On securely-configured servers,
 * the Update manager will require the administrator to provide SSH or FTP
 * credentials before allowing the installation to proceed; this allows the
 * site to update the new files as the user who owns all the Drupal files,
 * instead of as the user the webserver is running as. On servers where the
 * webserver user is itself the owner of the Drupal files, the administrator
 * will not be prompted for SSH or FTP credentials (note that these server
 * setups are common on shared hosting, but are inherently insecure).
 *
 * Some sites might wish to disable the above functionality, and only update
 * the code directly via SSH or FTP themselves. This setting completely
 * disables all functionality related to these authorized file operations.
 *
 * @see http://drupal.org/node/244924
 *
 * Remove the leading hash signs to disable.
 */
$conf['allow_authorize_operations'] = FALSE;

/**
 * Change the frequency of updates.
 * This also determines how often the cache is cleared/reset.
 * The unit is in days.
 */
$conf['update_check_frequency'] = 32;

/**
 * Force Clean URLS
 **/
$conf['clean_url'] = TRUE;

/**
 * Temporary directory
 **/
$conf['file_temporary_path'] = '/var/www/webfiles/temporary/sandbox';


/**
 * Assign php.ini variables based on  uid, role, or ip address.
 * Uid has priority over roles.
 * IP address has priority over uids.
 * Order of operations do apply.
 */
$conf['role_php_ini'] = array();
$conf['role_php_ini']['memory_limit'] = array('1' => '192M', '2' => '256M', '3' => '512M');
$conf['role_php_ini']['max_execution_time'] = array('1' => '120', '2' => '150', '3' => '210');
$conf['role_php_ini']['max_input_time'] = array('1' => '60', '2' => '75', '3' => '105');
$conf['role_php_ini']['include_path'] = array('1' => '/var/www/drupal/sandbox:/usr/local/lib/php', '2' => '/var/www/drupal/sandbox:/usr/local/lib/php');

$conf['user_php_ini'] = array();

$conf['ip_php_ini'] = array();
$conf['ip_php_ini']['memory_limit'] = array('127.0.0.1' => '512M');
$conf['ip_php_ini']['max_execution_time'] = array('127.0.0.1' => '960');
$conf['ip_php_ini']['max_input_time'] = array('127.0.0.1' => '960');
$conf['ip_php_ini']['include_path'] = array('127.0.0.1' => '/var/www/drupal/sandbox:/usr/local/lib/php');


/**
 * Batch Operation Overrides
 */
$conf['batch_foreground'] = array();
$conf['batch_foreground'][] = 'webform_results_download_form';
$conf['batch_background'] = array();


/**
 * Workbench access settings
 */
$conf['workbench_access_label'] = "Group";


/**
 * Webform default settings
 */
$conf['webform_export_encoding'] = 'UTF-8';


/**
 * Workbench menu default settings
 */
$conf['workbench_menu_item_use_normal_path'] = TRUE;


/**
 * McNeese Content Management Setings
 */
$conf['mcneese_management_group_leader_unassigned'] = 197; # 197 = nobody


/**
 * Disable HTML 5 tools chrome frame header
 */
$conf['html5_tools_add_chrome_frame_header'] = 0;


/**
 * Allow any/all attributes to be specified in the media_wysiwyg tag.
 */
$conf['media_wysiwyg_wysiwyg_allowed_attributes'] = NULL;


/**
 * restrict/reduce ultimate crons memory reservation code.
 */
$conf['ultimate_cron_reserve_memory'] = 1;


/**
 * Additional file_db variables.
 */
$conf['file_db_server_id'] = 2;
$conf['file_entity_default_scheme'] = 'dbu';
$conf['image_style_default_scheme'] = 'public';
$conf['phplot_api_default_scheme'] = 'public';


/**
 *  GNU PG Support
 */
putenv('GNUPGHOME=/var/www/drupal/sandbox/sites/default/gnupg');
$conf['gpg-mail'] = array(
  'no-reply@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
  'webmaster@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
  'financialaid@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
  'internships@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
  'studentunion@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
  'studentlife@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
  'internationaloffice@mcneese.edu' => array(
    'fingerprint' => '8FB5449D91992AF6AD8C1751BD57A1F7557EC223',
    'short_id' => '557EC223',
  ),
);

/**
 * e-mail specific settings.
 */
$conf['mail_system'] = array('default-system' => 'DefaultMailSystem');
#$conf['mail_system'] = array('default-system' => 'McNeeseMail');
#$conf['mail_system_includes'] = array('');
$conf['system_mail_id'] = 'sandbox.mcneese.edu';
$conf['system_mail_name'] = 'McNeese State University';


/**
 * Static registry support
 */
if (file_exists(DRUPAL_ROOT . '/sites/default/registry-sandbox.php')) {
  include_once DRUPAL_ROOT . '/sites/default/registry-sandbox.php';
}


/**
 * McNeese Feeds
 */
$conf['feed_groups_blacklist'] = array(
  'groups' => array(
    428 => 'Index',
    282 => 'About McNeese',
    668 => 'Maps',
    201 => 'Campus Maps',
    703 => 'Search',
    414 => 'Announcement',
    699 => 'Disclaimer',
    415 => 'News & Events',
    246 => 'Library',
    686 => '75th Anniversary',
    427 => 'Documentation',
    365 => 'FAQ',
    713 => 'Job Opportunity',
    381 => 'Calendar',
    400 => 'Athletics',
  ),
  'nodes' => array(
    NULL => array(
    ),

    // job opportunity sub-groups, ignore main pages.
    714 => array(
      6281,
    ),
    717 => array(
      6282,
    ),
    716 => array(
      6283,
    ),
    715 => array(
      6284,
    ),
  ),
);
$conf['feed_all_blacklist'] = array(
  'types' => array(
    'poster',
    'side_panel',
  ),
);


/**
 * Ldap blacklisting.
 */
$conf['ldap_user_blacklist']['mcneese_ldap']= array(
  'names' => array(
    'admin',
    'administer',
    'administrator',
    'anonymous',
    'person',
    'adapless',
    'kdapless',
    'pdapless',
    'ldapless',
    'unknown',
    'primary_system_control',
  ),
  'regex' => array(
    '/^(other|test|oth|om)-.*$/i',
  ),
  #'attribute' => array(
  #),
  #'no-attribute' => array(
  #  'employeenumber',
  #),
);


/**
 * Custom Error Pages
 */
$conf['error_document_file_403'] = '/var/www/error_documents/4xx/sandbox/403.html';
$conf['error_document_file_404'] = '/var/www/error_documents/4xx/sandbox/404.html';
$conf['error_document_file_500'] = '/var/www/error_documents/5xx/sandbox/500.html';
$conf['error_document_file_500-no_database'] = FALSE;
$conf['error_document_file_503'] = '/var/www/error_documents/5xx/sandbox/503.html';
#$conf['error_document_custom_menu_1_logged_in'] = NULL;
#$conf['error_document_custom_menu_2_logged_in'] = NULL;
$conf['error_document_load_blocks'] = TRUE;
$conf['error_document_functions'] = array(
  'mcneese_www_process_side_panel',
);

$conf['error_document_file-content_unavailable-campus_maps'] = '/var/www/error_documents/content_unavailable/www/campus_maps.html';
$conf['error_document_file-content_unavailable-final_exams'] = '/var/www/error_documents/content_unavailable/www/final_exams.html';
$conf['error_document_file-content_unavailable-fee_payments'] = '/var/www/error_documents/content_unavailable/www/fee_payments.html';
$conf['error_document_file-content_unavailable-emergency_information'] = '/var/www/error_documents/content_unavailable/www/emergency_information.html';
$conf['error_document_file-content_unavailable-emergency_information-hurricane'] = '/var/www/error_documents/content_unavailable/www/emergency_information-hurricane.html';
$conf['error_document_file-content_unavailable-ada_information'] = '/var/www/error_documents/content_unavailable/www/ada_information.html';

$conf['error_document_file-content_unavailable-final_exams-page_title'] = 'Fall 2016 Final Exam Schedule';
$conf['error_document_file-content_unavailable-final_exams-page_class_body'] = 'path-node_6524 alias-schedule_fall2016_final_exam_schedule node-id-6524 node-path-schedule_fall2016_final_exam_schedule alias-part-1-fall2016 alias-part-2-final_exam_schedule node-theme-page-650';
$conf['error_document_file-content_unavailable-final_exams-page_class_section'] = 'node-id-6524 node-theme-page-650 ';

$conf['error_document_file-content_unavailable-fee_payments-title'] = 'Fall 2016 Registration Fees';
$conf['error_document_file-content_unavailable-fee_payments-page_class_body'] = 'path-node_6529 alias-schedule_fall2016_fee_payment node-id-6529 node-type-page node-path-schedule_fall2016_fee_payment alias-part-0-schedule alias-part-1-fall2016 alias-part-2-fee_payment node-theme-page-650 ';
$conf['error_document_file-content_unavailable-fee_payments-page_class_section'] = 'node-id-6529 node-theme-page-650 ';

$conf['error_document_file-content_unavailable-GetAGamePlan.jpg'] = '/var/www/error_documents/content_unavailable/www/files/GetAGamePlan.jpg';
$conf['error_document_file-content_unavailable-page_white_acrobat.png'] = '/var/www/error_documents/content_unavailable/www/files/page_white_acrobat.png';
$conf['error_document_file-content_unavailable-EPT_Phases.pdf'] = '/var/www/error_documents/content_unavailable/www/files/EPT_Phases.pdf';
$conf['error_document_file-content_unavailable-Hurricane_Checklist.pdf'] = '/var/www/error_documents/content_unavailable/www/files/Hurricane_Checklist.pdf';
$conf['error_document_file-content_unavailable-Student_Disaster_Preparedness_Tips.pdf'] = '/var/www/error_documents/content_unavailable/www/files/Student_Disaster_Preparedness_Tips.pdf';
$conf['error_document_file-content_unavailable-Hurricane_Emergency_Operations_Plan.pdf'] = '/var/www/error_documents/content_unavailable/www/files/Hurricane_Emergency_Operations_Plan.pdf';

// static front page settings
$conf['mcneese_static_file_frontpage'] = '/tmp/static-sandbox/frontpage.html';
$conf['mcneese_static_file_frontpage-https'] = '/tmp/static-sandbox/frontpage-https.html';
$conf['mcneese_static_file_frontpage-expires'] = '+3 hours';


// enable all errors
#error_reporting(E_ALL);
#ini_set('display_errors', TRUE);
#ini_set('display_startup_errors', TRUE);

// manually turn on maintenance mode
#$conf['maintenance_mode'] = TRUE;

// disable the front page featured region
#$conf['disable_frontpage_featured'] = TRUE;

// turn off opcache for this site.
#ini_set('opcache.enable', FALSE);
#ini_set('opcache.enable_cli', FALSE);
