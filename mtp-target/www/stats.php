<?php



	$fp = fopen("../server/connection.stat", "r");
	if (!$fp) {echo "<p>Unable to open remote file.</p>"; exit;}
	
	$userCountPerHourTotal;
	$userCountPerHourToday;
	$userCountPerHourTodayMax;
	$userCountPerMinuteTodayMax;
	$lastHour = 0;
	$lastDay = 0;
	$lastMinute = 0;
	$once = 0;
	$firstDate = mktime();
	$lastRestartTime = mktime();
	
	for($i=0;$i<24;$i++)
	{
		$userCountPerHourTotal[$i]=0;
		$userCountPerHourToday[$i]=0;
		$userCountPerHourTodayMax[$i]=0;
	}
	$userCountPerMinuteTodayMax = array_fill(0,60,-1);
	
	$playerCount = 0;
	while (!feof($fp)):
		$line = fgets($fp, 2048);
		$out = array(substr($line,0,strlen($line)-1));//remove \n
		list ($ttime, $syear, $smonth, $sday, $shour, $smin, $ssec, $inout, $sname)= split (" ", $out[0]);

		$name = str_replace("'","",$sname);

		//printf("[%s] -> [%s]<br>",$sname,$name);
		
		$hour = intval($shour);
		$minute = intval($smin);
		$sec = intval($ssec);
		$day = intval($sday);
		$month = intval($smonth);
		$year = intval($syear);
		if(strlen($line)!=0)
		{		
			if($lastHour!=$hour)
			{
				//printf("%dh0 = %d<br>",$lastHour,$userCountPerHourTotal[$lastHour]);
				$lastHour=$hour;
				$userCountPerMinuteTodayMax = array_fill(0,60,-1);
				$userCountPerMinuteTodayMax[0]=$playerCount;
				/*
				for($i=0;$i<60;$i++)
				{
					printf("%d<br>",$userCountPerMinuteTodayMax[$i]);
				}
				printf("<hr>");
				*/
			}
			
			if($lastDay!=$day)
			{
				$lastDay=$day;
				for($i=0;$i<24;$i++)
				{
					$userCountPerHourToday[$i]=0;
					$userCountPerHourTodayMax[$i]=0;
				}
				//printf("<hr>");
			}
			
			if($inout=='+')
			{
				$lastMinute = $minute;
				$playerCount++;
				$userCountPerHourTotal[$hour]++;
				$userCountPerHourToday[$hour]++;
				if($playerCount>$userCountPerHourTodayMax[$hour])
					$userCountPerHourTodayMax[$hour] = $playerCount;
				if($playerCount>$userCountPerMinuteTodayMax[$minute])
					$userCountPerMinuteTodayMax[$minute] = $playerCount;
				//printf("%d:%d [%s] comes(%d,%d,%d) in<br>",$hour,$minute,$name,$playerCount,$userCountPerHourToday[$hour],$userCountPerMinuteTodayMax[$minute]);
			}
			else if($inout=='#')
			{
				$lastRestartTime = mktime($hour,$minute,$sec,$month,$day,$year);
				if($once==0)
				{
					$firstDate = $lastRestartTime;
					$once = 0;
				}
				$playerCount = 0;
				//printf("<hr>",$name);
			}
			else if($inout=='?')
			{
				if(strlen($name)>0)
					$playerCount++;
				//printf("invalid login(%d) %s(%d)<br>",$playerCount,$name,strlen($name));
			}
			else if($inout=='-')
			{
				if(strlen($name)>0)
					$playerCount--;
				//printf("[%s](%d) leaves(%d)<br>",$name,strlen($name),$playerCount);
			}
		}
		$fp++;
	endwhile;
	
	fclose($fp);

	$t1 = strtotime(date("l dS of F Y H:i:s"));
	$t2 = strtotime(date("l dS of F Y H:i:s",$lastRestartTime));
	
	//printf("%d %d %d <br>",$t1-$t2, $t1,$t2);
	$serverUptime = mktime() - $lastRestartTime;
	//$serverUptime = $serverUptime - mktime(1);

	printf("Server uptime : ");
	if(date("z",$serverUptime)!="0")
		printf("%s day(s) %s<br>\n",date("z",$serverUptime),date("H:i:s",$serverUptime));
	else
		printf("%s<br>\n",date("H:i:s",$serverUptime));
	printf("Current logged user : %d<br>\n",$playerCount);
	
	printf("report date  : %s<br>\n",date("l dS of F Y H:i:s"));
	printf("Last restart : %s<br>\n",date("l dS of F Y H:i:s",$lastRestartTime));
	
?>

<br>
<br>

<?php
printf("<b>Total</b> login :<br>\n");
printf("Since %s\n",date("l dS of F Y h:i:s A",$firstDate));
?>

<table>
<tr valign="bottom">
<?php
	$maxTotal = 0;
	
	for($i=0;$i<24;$i++)
	{
		if($userCountPerHourTotal[$i]>$maxTotal)
			$maxTotal = $userCountPerHourTotal[$i];
	}
	
	for($i=0;$i<24;$i++)
	{
		//printf("=>%dh0 : %d<br>",$i,$userCountPerHourTotal[$i]);
		printf("<td valign=\"bottom\">\n");
		if($maxTotal!=0)
			printf("\t<img align=\"bottom\" src=\"./img/vp.png\" height=\"%d\" width=\"6\" alt='total login : %d' title='total login : %d' />\n",$userCountPerHourTotal[$i]*100/$maxTotal,$userCountPerHourTotal[$i],$userCountPerHourTotal[$i]);
		printf("</td>\n");
	}
	printf("</tr>\n");
	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td width=20>%d</td>\n",$userCountPerHourTotal[$i]);
	}
	printf("</tr>\n");	

	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
	}
	printf("</tr>\n");	
	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td>%d</td>\n",$i);
	}
	printf("</tr>\n");	

?>
</table>

<br>
<br>

Today login :
<table>
<tr valign="bottom">
<?php
	$maxToday = 0;
	
	for($i=0;$i<24;$i++)
	{
		if($userCountPerHourToday[$i]>$maxToday)
			$maxToday = $userCountPerHourToday[$i];
		
	}
	
	for($i=0;$i<24;$i++)
	{
		printf("<td valign=\"bottom\">\n");
		if($maxToday!=0)
			printf("\t<img align=\"bottom\" src=\"./img/vh.png\" height=\"%d\" width=\"6\" alt='today login : %d' title='today login : %d' />\n",$userCountPerHourToday[$i]*100/$maxToday,$userCountPerHourToday[$i],$userCountPerHourToday[$i]);
		printf("</td>\n");
	}
	printf("</tr>\n");
	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td width=20>%d</td>\n",$userCountPerHourToday[$i]);
	}
	printf("</tr>\n");	
	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
	}
	printf("</tr>\n");	

	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td>%d</td>\n",$i);
	}
	printf("</tr>\n");	
	
?>
</table>

<br>
<br>


Today Max simultaneous user :
<table>
<tr valign="bottom">
<?php
	$maxTodayMax = 0;
	
	for($i=0;$i<24;$i++)
	{
		if($userCountPerHourTodayMax[$i]>$maxTodayMax)
			$maxTodayMax = $userCountPerHourTodayMax[$i];
		
	}
	
	for($i=0;$i<24;$i++)
	{
		printf("<td valign=\"bottom\">\n");
		if($maxTodayMax!=0)
			printf("\t<img align=\"bottom\" src=\"./img/vh.png\" height=\"%d\" width=\"6\" alt='today simultaneous user : %d' title='today simultaneous user : %d' />\n",$userCountPerHourTodayMax[$i]*100/$maxTodayMax,$userCountPerHourTodayMax[$i],$userCountPerHourTodayMax[$i]);
		printf("</td>\n");
	}
	printf("</tr>\n");
	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td width=20>%d</td>\n",$userCountPerHourTodayMax[$i]);
	}
	printf("</tr>\n");	
	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
	}
	printf("</tr>\n");	

	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td>%d</td>\n",$i);
	}
	printf("</tr>\n");	
	
?>
</table>

<br>
<br>

Last hour max simultaneous user :
<table>
<tr valign="bottom">
<?php
	$maxHourMax = 0;
	
	for($i=0;$i<60 && $i<$lastMinute;$i++)
	{
		if($userCountPerMinuteTodayMax[$i]==-1 && $i>0 && $i<$lastMinute)
			$userCountPerMinuteTodayMax[$i]=$userCountPerMinuteTodayMax[$i-1];
		if($userCountPerMinuteTodayMax[$i]>$maxHourMax)
			$maxHourMax = $userCountPerMinuteTodayMax[$i];
		
	}
	
	for($i=0;$i<60;$i++)
	{
		printf("<td valign=\"bottom\">\n");
		if($maxHourMax!=0)
			printf("\t<img align=\"bottom\" src=\"./img/vp.png\" height=\"%d\" width=\"6\" alt='today simultaneous user : %d' title='today simultaneous user : %d' />\n",$userCountPerMinuteTodayMax[$i]*100/$maxHourMax,$userCountPerMinuteTodayMax[$i],$userCountPerMinuteTodayMax[$i]);
		printf("</td>\n");
	}
	printf("</tr>\n");
	
	
	printf("<tr>\n");
	for($i=0;$i<60 && $i<=$lastMinute;$i++)
	{
		printf("<td>%d</td>\n",$userCountPerMinuteTodayMax[$i]);
	}
	printf("</tr>\n");	
	
	printf("<tr>\n");
	for($i=0;$i<60 && $i<=$lastMinute;$i++)
	{
		printf("<td>%02d</td>\n",$i);
	}
	printf("</tr>\n");	
	
	
	/*
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
	}
	printf("</tr>\n");
	*/
?>
</table>

