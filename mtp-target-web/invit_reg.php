<?php

// This the same page that displays the invitation and validate it (send an email)
// We know if we have to send an invitation because the post variables are available

echo "<h2>".lg('InvitationHeader')."</h2>";

if(CUser::instance()->nbInvitations()>0 && isset($_POST['invit_fn']) && isset($_POST['invit_ln']) && isset($_POST['invit_email']) && isset($_POST['invit_note']))
{
	// We have to send an invitation
	$email = $_POST['invit_email'];
	$key = md5(CUser::instance()->login() . time());
	
	if(validEmail($email)) {
		exec_game_db_requete("INSERT INTO invitation (UId, HashKey, FirstName, LastName, Email, Note) VALUES('".CUser::instance()->uid()."', '$key', ".smartQuote($_POST['invit_fn']).", ".smartQuote($_POST['invit_ln']).", '$email', ".smartQuote($_POST['invit_note']).")");
		if ($_POST['invit_fn'] != "") {
			$subject = $_POST['invit_fn'].", ";
		}
		$subject .= sprintf(lg("HasInvitedYou"), CUser::instance()->firstName(), CUser::instance()->lastName());
		$message = stripslashes($_POST['invit_note']);
		$message .= "\n\n-----------------------------------------------------------------------\n\n";
		$message .= sprintf(lg("InvitMailMessage"), CUser::instance()->firstName(), CUser::instance()->lastName(), "http://localhost/mtp-target-web/index.php?page=invit_reg&key=".$key, CUser::instance()->firstName(), CUser::instance()->lastName(), "http://localhost/mtp-target-web/index.php?page=home");
		mail($email, $subject, $message, "From: ".CUser::instance()->email()."\nX-Mailer: PHP/" . phpversion(), "-f ".CUser::instance()->email());
		CUser::instance()->decreaseNbInvitations();
		echo lg('InvitationSent')."<br/><br/>";
	} else {
		echo lg('BadEmail')."<br/><br/>";
	}
}

switch (CUser::instance()->nbInvitations())
{
	case 0: echo lg('NoInvitationLeft'); return; break;
	case 1: echo lg('LastInvitation'); break;
	default: printf(lg('YouHaveNInvitationsLeft'), CUser::instance()->nbInvitations()); break;
}

echo ".<br/><br/>".lg('InvitationText');
echo "<br/><br/>".lg('InvitationSendTo');

echo '<form method="post" action="?page=invit_send" name="loginForm"><table class="stat">';
echo '<tr><td>'.lg('FirstName').':</td><td><input type="text" name="invit_fn" value="vianney" size="20"/></td></tr>';
echo '<tr><td>'.lg('LastName').':</td><td><input type="text" name="invit_ln" value="lecroart" size="20"/></td></tr>';
echo '<tr><td>'.lg('Email').':</td><td><input type="text" name="invit_email" value="acemtp@gmail.com" size="40"/></td></tr>';
echo '<tr><td colspan="2">'.lg('Note').':</td></tr><tr><td colspan="2"><textarea name="invit_note" rows="5" cols="50">'.lg('InvitationNote').'</textarea></td></tr>';
echo '<tr><td><input type="submit" value="'.lg('SendInvitation').'" /></td></tr>';
echo '</table></form>';

?>
