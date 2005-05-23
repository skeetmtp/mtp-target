<?php

die("REMOVED BY ACE. OLD CODE ".__FILE__);
/*

  echo "<br>";
  echo "<table border=\"0\" cellpadding=\"1\" cellspacing=\"0\"";// width=\"100%\">";
  echo "<tr>";
  echo "<th></th>";
  echo "<th align=\"left\"></th>";
  echo "<th></th>";
  echo "<th></th>";

  $fp = fopen("news.txt", "r");
  if(!$fp)
    echo "could not access news file\n";
  else
  {
    $i=0;
    while(!feof($fp))
    {
      $fline=fgets($fp,128);
      $comment = substr(strstr($fline,"by"),2);
      if($comment)
      {
        $tab[$i] = $fline;
        $i++;
      }
    }
    fclose($fp);
    $lastDate = "";
    $lastComment = "";
    for ($i = count ($tab) - 1 ; $i >= 0; $i-- )
    {
      $fline = $tab[$i];
      $comment = substr(strstr($fline,"by"),2);
      $fdate = substr($fline,0,10);
      $startComment = strlen($comment)-strlen(strstr($comment,":"));
      $fname = substr($comment,0,$startComment);
      $fcomment = substr(strstr($comment,":"),1);
      if($lastComment != $fcomment)
      {
        if(strlen($fcomment)>3)
        {
          if($lastDate != $fdate)
          {
            echo "<tr><td><h6><br></td><td></td><td></td></tr>";
          }
          echo "<tr>";
          echo "<TD bgColor=#52789e><SPACER width=\"1\" height=\"1\" type=\"block\"></SPACER></TD>";
          echo "<td><h5>";
          if($lastDate != $fdate)
          {
            echo $fdate;
            echo "<br>";
          }
          echo "<FONT color=#777777>" . $fname . "</FONT>";
          echo "</td><td colspan=\"10\"></td><td><h5>";
          if($lastDate != $fdate)
            echo " <br>";
          echo $fcomment;
          $lastDate = $fdate;
          $lastComment = $fcomment;
          echo "</tr>";
        }
      }
    }
  }
  echo "</tr>";
  echo "<table>";
*/
?>

