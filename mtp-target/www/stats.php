<?php
	$fp = fopen("../server/connection.stat", "r");
	if (!$fp) {echo "<p>Unable to open remote file.</p>"; exit;}
	
	$userCountPerHourTotal;
	$userCountPerHourToday;
	$userCountPerHourTodayMax;
	$lastHour = 0;
	$lastDay = 0;
	
	$lastRestartTime = mktime();
	
	for($i=0;$i<24;$i++)
	{
		$userCountPerHourTotal[$i]=0;
		$userCountPerHourToday[$i]=0;
		$userCountPerHourTodayMax[$i]=0;
	}

	
	$playerCount = 0;
	while (!feof($fp)):
		$line = fgets($fp, 2048);
		$out = array(substr($line,0,strlen($line)-1));//remove \n
		list ($ttime, $year, $month, $sday, $shour, $smin, $ssec, $inout, $sname)= split (" ", $out[0]);

		$name = str_replace("'","",$sname);

		//printf("[%s] -> [%s]<br>",$sname,$name);
		
		$hour = intval($shour);
		$min = intval($smin);
		$sec = intval($ssec);
		$day = intval($sday);
		if(strlen($line)!=0)
		{		
			if($lastHour!=$hour)
			{
				//printf("%dh0 = %d<br>",$lastHour,$userCountPerHourTotal[$lastHour]);
				$lastHour=$hour;
			}
			
			if($lastDay!=$day)
			{
				$lastDay=$day;
				for($i=0;$i<24;$i++)
				{
					$userCountPerHourToday[$i]=0;
					$userCountPerHourTodayMax[$i]=0;
				}
				//printf("<hr>",$name);
			}
			
			if($inout=='+')
			{
				$playerCount++;
				$userCountPerHourTotal[$hour]++;
				$userCountPerHourToday[$hour]++;
				if($playerCount>$userCountPerHourTodayMax[$hour])
					$userCountPerHourTodayMax[$hour] = $playerCount;
				//printf("%d:%d [%s] comes(%d,%d) in<br>",$hour,$min,$name,$playerCount,$userCountPerHourToday[$hour]);
			}
			else if($inout=='#')
			{
				$lastRestartTime = mktime($hour,$min,$sec);
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

	$serverUptime = mktime() - $lastRestartTime;

	printf("server uptime : ");
	if(date("z",$serverUptime)!="0")
		printf("%s day(s) %s<br>\n",date("z",$serverUptime),date("H:i:s",$serverUptime));
	else
		printf("%s<br>\n",date("H:i:s",$serverUptime));
	printf("Courrent logged user : %d<br>\n",$playerCount);
	
?>

<br>
<br>

Total login :

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
		printf("<td>%d</td>\n",$i);
	}
	printf("</tr>\n");	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
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
		printf("<td>%d</td>\n",$i);
	}
	printf("</tr>\n");	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
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
		if($maxToday!=0)
			printf("\t<img align=\"bottom\" src=\"./img/vh.png\" height=\"%d\" width=\"6\" alt='today simultaneous user : %d' title='today simultaneous user : %d' />\n",$userCountPerHourTodayMax[$i]*100/$maxTodayMax,$userCountPerHourTodayMax[$i],$userCountPerHourTodayMax[$i]);
		printf("</td>\n");
	}
	printf("</tr>\n");
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td>%d</td>\n",$i);
	}
	printf("</tr>\n");	
	printf("<tr>\n");
	for($i=0;$i<24;$i++)
	{
		printf("<td><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
	}
	printf("</tr>\n");	
?>
</table>

