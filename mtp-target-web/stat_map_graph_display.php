<?php

function drawMapUsage($html_fp,$mysqlArray,$CriterionBase,$valCount,$header,$CriterionArray,$link)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;
	global $table_news_row_color_2;

/*
	$mysqlArray = $result; 
	$CriterionBase = 1;
	$valCount = 15;
	$header = "Level usage";
	$CriterionArray = array("", "Level");
*/	
	$valCount += $CriterionBase;

	for($i=$CriterionBase;$i<$valCount;$i++)
		for($j=0;$j<count($CriterionArray)+1;$j++)
			$valPerCriterion[$i][$j]=0;
	while($line = mysql_fetch_array($mysqlArray))
	{
		$valPerCriterion[$line[1]][0]=$line[0];
		for($j=0;$j<count($CriterionArray);$j++)
			$valPerCriterion[$line[1]][$j+1]=$line[$j+1];
	}

	$maxTotal = 0;
	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		if($valPerCriterion[$i][0]>$maxTotal)
			$maxTotal = $valPerCriterion[$i][0];
	}

	fprintf($html_fp,"<table width=\"\" border=\"0\" cellpadding=\"0\" cellspacing=\"1\"  bgcolor=\"%s\">\n",$table_news_bgcolor_color);
	fprintf($html_fp,"<th bgcolor=\"%s\" align=\"center\" >\n",$table_news_head_color);
	fprintf($html_fp,"%s\n",$header);
	fprintf($html_fp,"</th>\n");
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	fprintf($html_fp,"<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"3\" bgcolor=\"%s\">\n",$table_news_row_color);
	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		$v = $valPerCriterion[$i][0];
		if($v>0)
		{
			fprintf($html_fp,"<tr>\n");
			fprintf($html_fp,"<td>\n");
			$mapName = $valPerCriterion[$i][2];
			$mapId = $valPerCriterion[$i][1];
			if($mapName=="")
				$mapName = mapId2Name($i); 
			$mapLink = sprintf($link,$mapId,$mapName);
			fprintf($html_fp,"<b>%s</b>\n",$mapLink);
			fprintf($html_fp,"</td>\n");
	
			fprintf($html_fp,"<td align=\"right\">\n");
			fprintf($html_fp,"%d\n",$valPerCriterion[$i][0]);
			fprintf($html_fp,"</td>\n");
			
			fprintf($html_fp,"<td>\n");
			fprintf($html_fp,"<img align=\"bottom\" src=\"./img/vp.png\" height=\"6\" width=\"%d\" alt='value : %d' title='value : %d' />\n",100*$v/$maxTotal,$v,$v);
			fprintf($html_fp,"</td>\n");
	
	
			fprintf($html_fp,"</tr>\n");
		}
	}	
	
	fprintf($html_fp,"</table>\n");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
	fprintf($html_fp,"<br>\n");
}

?>
