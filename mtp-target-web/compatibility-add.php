<?php
include_once("compatibility-func.php");

compatibilityAddEntry($type,$problem,$context,"unread",$workaround,"0");

            $requete = sprintf("SELECT * FROM user;");
            $resultat = exec_requete($requete);
            $count = 0;
            
            while ($ligne = mysql_fetch_array($resultat))
            {
             $announce_subject = "mtp-target new bug submited";
             $announce_from_email = "skeet@planet-d.net";
             $announce_from_name = "mtp-target web site";
             $announce_to_email = $ligne[2];//"skeet@teamchman.com";
             $body = "";
             $body .= "from " . $REMOTE_HOST . " ";
             $body .= date("l dS of F Y h:i:s A");
             $body .= " using " . $HTTP_USER_AGENT ."\n";
             $body .= "problem : " . $problem . "\n";
             $body .= "context : " . $context . "\n";
             $body .= "workaround : " . $workaround . "\n";
             $MP = "/usr/sbin/sendmail -t";
             $spec_envelope = 1;
             // Access Sendmail
             // Conditionally match envelope address
             if($spec_envelope)
             {
              $MP .= " -f $announce_from_email";
             }
             $fd = popen($MP,"w");
             fputs($fd, "To: $announce_to_email\n");
             fputs($fd, "From: $announce_from_name <$announce_from_email>\n");
             fputs($fd, "Subject: $announce_subject\n");
             fputs($fd, "X-Mailer: PHP4\n");
             fputs($fd, str_replace(":", "=", $body));
             pclose($fd);
            }

header("Location: index.php?page=compatibility-list.php");

?>



