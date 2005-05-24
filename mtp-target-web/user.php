<?php

require_once('mysql-func.php');
require_once('config.php');

class CUser {

	static private $Instance = false;

	static public function instance() { 
		if(!CUser::$Instance) { 
			CUser::$Instance = new CUser(); 
		} 
		return CUser::$Instance; 
	} 
   
	private function __construct() {

		$this->cleanDeprecatedCookieVariables();
		
		// User want to logout
		if (isset($_GET['logout'])) {
			$this->logout();
		    header("Location: index.php");
		}

		// If there's no session, create default values for the session	
		if (!isset($_SESSION['UId']) ) {
			$this->_sessionDefaults();
		}

		// First, try to find the language of this user
		$this->negociateLanguage();
		
		// Second, check if the user is logged or not
		if ($_SESSION['Logged']) {
			// We already have a logged session, nothing to do
			$this->dbg .= " logged *";
			return;
		}

		if (isset($_POST['LoginLogin']) && isset($_POST['LoginPassword'])) {
			// The user tries to log in, we have login values, let check is they are good
			$remember = isset($_POST['LoginRemember']) && $_POST['LoginRemember'] == "yes";
			$this->dbg .= " posted a login $remember*";
			$this->_checkLogin($_POST['LoginLogin'], $_POST['LoginPassword'], $remember);
			return;
		}

		if (isset($_COOKIE['PermanentLogin']) ) {
			// We have a cookie that tell that the user want to automatic log, let check if the cookie seems good
			$this->_checkRemembered($_COOKIE['PermanentLogin']);
			$this->dbg .= " logged by permanent login *";
			return;
		}

		// There s an anonymous guy on the site
		$this->dbg .= " anonym guy *";
	}

	public function displayLoginForm() {
		echo '<form method="post" action="">';
		if($this->Error != "") echo '<tr><td></td><td align="center">'.$this->Error.'</td></tr>';
		echo '<tr><td class="r">'.lg('Login').':</td><td class="l"><input type="text" name="LoginLogin" size="16"/></td></tr>';
		echo '<tr><td class="r">'.lg('Password').':</td><td class="l"><input type="password" name="LoginPassword" size="16"/></td></tr>';
		echo '<tr><td class="r"><input type="checkbox" name="LoginRemember" value="yes"/></td><td class="l">'.lg('RememberPassword').'</td></tr>';
		echo '<tr><td class="r"><a href="?page=user_registration">'.lg('Register').'</a></td><td class="r">';
		echo '<input type="submit" value="'.lg('SignIn').'" /></td></tr></form>';
	}

	public function logged() { return $_SESSION['Logged']; }
	public function login() { return $_SESSION['Login']; }
	public function uid() { return $_SESSION['UId']; }
	public function admin() { return $_SESSION['Admin']; }

	public function language() { return $this->Language; }

	public $dbg = "";
	private $Language= "en";
	private $Error= "";
	
	private function _sessionDefaults() {
		$this->dbg .= " set default session values *";
		$_SESSION['Logged'] = false;
		$_SESSION['UId'] = 0;
		$_SESSION['Login'] = '';
		$_SESSION['Admin'] = false;
	}
	
	private function setSession($UId,$Login)
	{
		$_SESSION['UId'] = $UId;
		$_SESSION['Login'] = $Login;
		$_SESSION['Logged'] = true;
		if(strcasecmp($Login , "ace") == 0) $_SESSION['Admin'] = true;
	}
	
	private function _checkLogin($Login, $Password, $Remember) {
		$Result = exec_requete("SELECT * FROM user WHERE Login = ".smartQuote($Login));

		if ($Line = mysql_fetch_array($Result)) {
			if(crypt($Password, $Line['Password']) == $Line['Password']) {
				$this->setSession($Line['UId'], $Line['Login']);

				if ($Remember) {
					$Cookie = md5(uniqid(mt_rand(), true));
					$PackedCookie = serialize(array($Line['Login'], $Cookie));
					setcookie('PermanentLogin', $PackedCookie, time() + 60*60*24*30); // 30 days
					exec_requete("UPDATE user SET WebCookie = ".smartQuote($Cookie)." WHERE UId = ".smartQuote($_SESSION['UId']));				
				}
				$this->dbg .= " pass ok *";
				return true;
			} else {
				$this->dbg .= " bad pass *";
				$this->Error = lg('BadPassword');
			}
		} else {
			$this->dbg .= " login not found in db *";
			$this->Error = lg('BadLogin');
		}
		$this->dbg .= " pass failed *";
		$this->_sessionDefaults();
		return false;
	}

	public function logout()
	{
		$_SESSION = array();
		if (isset($_COOKIE['PermanentLogin'])) {
			setcookie('PermanentLogin', '', time()-42000);
			unset($_COOKIE['PermanentLogin']);
		}
		session_destroy();
		$this->_sessionDefaults();
	}

	private function _checkRemembered($PackedCookie) {
		list($Login, $Cookie) = unserialize(stripslashes($PackedCookie));
		if (!$Login or !$Cookie) {
			$this->dbg .= " bad unserial - $PackedCookie - $Login - $Cookie *";
			$this->logout();
			return;
		}
		$Result = exec_requete("SELECT * FROM user WHERE (Login = ".smartQuote($Login).") AND (WebCookie = ".smartQuote($Cookie).")");

		if ($Line = mysql_fetch_array($Result)) {
			$this->setSession($Line['UId'], $Line['Login']);
			$this->dbg .= " pass ok via remember *";		
		} else {
			$this->dbg .= " pass failed via remember *";
			$this->logout();
		}
	}

	private function negociateLanguage() {
		if (isset($_GET['lang'])) {
			$this->Language = $_GET['lang'];
			setcookie('Language', $this->Language, time() + 60*60*24*30); // 30 days
			$this->dbg .= " from post *";
		} elseif (isset($_COOKIE['Language'])) {
			$this->Language = $_COOKIE['Language'];
			$this->dbg .= " from cookie *";
		} elseif (isset($_SERVER['HTTP_ACCEPT_LANGUAGE'])) {
			if(strstr($_SERVER['HTTP_ACCEPT_LANGUAGE'],"en"))
				$this->Language = "en";
			if(strstr($_SERVER['HTTP_ACCEPT_LANGUAGE'],"fr"))
				$this->Language = "fr";
			$this->dbg .= " from http accept language *";
		} else {
			$this->Language = "en";
			$this->dbg .= " from default *";
		}
		$this->dbg .= " lang=$this->Language *";
	}

	// On the old web site, passwords are stored on client side in clear format
	// We remove all these cookie variables
	private function cleanDeprecatedCookieVariables() {
		if (isset($_COOKIE['mtp_target_lang'])) setcookie('mtp_target_lang', '', time()-42000);
		if (isset($_COOKIE['mtp_target_user_id'])) setcookie('mtp_target_user_id', '', time()-42000);
		if (isset($_COOKIE['mtp_target_user_login'])) setcookie('mtp_target_user_login', '', time()-42000);
		if (isset($_COOKIE['mtp_target_user_password'])) setcookie('mtp_target_user_password', '', time()-42000);
		if (isset($_COOKIE['mtp_target_admin_login'])) setcookie('mtp_target_admin_login', '', time()-42000);
		if (isset($_COOKIE['mtp_target_admin_password'])) setcookie('mtp_target_admin_password', '', time()-42000);
	}


}
