<?php 

function drawGraph($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionName)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;

	if($isHour)
	{
		$CriterionBase = 0;
		$valCount = 24;
	}

	$valCount += $CriterionBase;

	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		$valPerCriterion[$i][0]=0;
		$valPerCriterion[$i][1]=$i;
	}
	while($line = mysql_fetch_array($mysqlArray))
	{
		$valPerCriterion[$line[1]][0]=$line[0];
		$valPerCriterion[$line[1]][1]=$line[1];
	}

	$CriterionArray[0] = $CriterionName;
	drawGraphInner($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,false);
	
	fprintf($html_fp,"<br/>\n");
	
}

function drawGraphLink($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionName,$CriterionLink)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;

	if($isHour)
	{
		$CriterionBase = 0;
		$valCount = 24;
	}

	$valCount += $CriterionBase;

	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		$valPerCriterion[$i][0]=0;
		$valPerCriterion[$i][1]=$i;
	}
	while($line = mysql_fetch_array($mysqlArray))
	{
		$valPerCriterion[$line[1]][0]=$line[0];
		$valPerCriterion[$line[1]][1]=sprintf($CriterionLink,$line[1],$line[1]);
	}

	$CriterionArray[0] = $CriterionName;
	drawGraphInner($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,false);
	
	fprintf($html_fp,"<br/>\n");
	
}

function drawGraphH($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionName)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;

	if($isHour)
	{
		$CriterionBase = 0;
		$valCount = 24;
	}

	$valCount += $CriterionBase;

	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		$valPerCriterion[$i][0]=0;
		$valPerCriterion[$i][1]=$i;
	}
	while($line = mysql_fetch_array($mysqlArray))
	{
		$valPerCriterion[$line[1]][0]=$line[0];
		$valPerCriterion[$line[1]][1]=$line[1];
	}

	$CriterionArray[0] = $CriterionName;
	drawGraphInnerHorizontal($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,false);
	
	fprintf($html_fp,"<br/>\n");
	
}

function drawGraphMultiple($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;

	if($isHour)
	{
		$CriterionBase = 0;
		$valCount = 24;
	}

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
	
	drawGraphInner($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull);
	
	fprintf($html_fp,"<br/>\n");
	
}

function drawGraphMultipleLink($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull,$CriterionLink)
{
	drawGraphMultipleLinkCount($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull,$CriterionLink,count($CriterionArray)-1);
}

function drawGraphMultipleLinkCount($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull,$CriterionLink,$linkParamCount)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;

	if($isHour)
	{
		$CriterionBase = 0;
		$valCount = 24;
	}

	$valCount += $CriterionBase;

	for($i=$CriterionBase;$i<$valCount;$i++)
		for($j=0;$j<count($CriterionArray)+1;$j++)
			$valPerCriterion[$i][$j]=0;
	while($line = mysql_fetch_array($mysqlArray))
	{
		$valPerCriterion[$line[1]][0]=$line[0];
		
		switch($linkParamCount)
		{
			case 1:
				$valPerCriterion[$line[1]][2]=sprintf($CriterionLink,$line[2],$line[2]);
				break;
			case 2:
				$valPerCriterion[$line[1]][2]=sprintf($CriterionLink,$line[2],$line[3],$line[2]);
				break;
			case 3:
				$valPerCriterion[$line[1]][2]=sprintf($CriterionLink,$line[2],$line[3],$line[4],$line[2]);
				break;
			default:
				$valPerCriterion[$line[1]][2] = $line[2];
				break;
		}
		for($j=0;$j<count($CriterionArray);$j++)
			if($j!=1)
				$valPerCriterion[$line[1]][$j+1]=$line[$j+1];
		/*
		for($j=0;$j<count($CriterionArray);$j++)
			$valPerCriterion[$line[1]][$j+1]=sprintf($CriterionLink,$line[$j+1],$line[$j+1]);
			*/
	}
	
	drawGraphInner($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull);
	
	fprintf($html_fp,"<br/>\n");
	
}


function drawGraphMultipleH($html_fp,$mysqlArray,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull)
{
	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;

	if($isHour)
	{
		$CriterionBase = 0;
		$valCount = 24;
	}

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
	
	drawGraphInnerHorizontal($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull);
	
	fprintf($html_fp,"<br/>\n");
	
}

function drawGraphInner($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull)
{
	$maxTotal = 0;
	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		if($valPerCriterion[$i][0]>$maxTotal)
			$maxTotal = $valPerCriterion[$i][0];
	}

	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;
	
	fprintf($html_fp,"<table class=\"stat\">\n");
	fprintf($html_fp,"<th>\n",$table_news_head_color);
	fprintf($html_fp,"%s\n",$header);
	fprintf($html_fp,"</th>\n");
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	fprintf($html_fp,"<table>\n");
	fprintf($html_fp,"<tr valign=\"bottom\">\n");

	
	fprintf($html_fp,"<td align=\"left\" valign=\"top\">max=%d\n",$maxTotal);
	fprintf($html_fp,"</td>\n");
	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		fprintf($html_fp,"<td align=\"center\" valign=\"bottom\">\n");
		if($maxTotal!=0)
		{
			$v = $valPerCriterion[$i][0];
			fprintf($html_fp,"\t<img align=\"bottom\" src=\"./img/vp.png\" height=\"%d\" width=\"6\" alt='value : %d' title='value : %d' />\n",100*$v/$maxTotal,$v,$v);
		}
		fprintf($html_fp,"</td>\n");
	}
	fprintf($html_fp,"</tr>\n");
	
	//if($displayValue)
	{
			fprintf($html_fp,"<tr class=\"expandable\" border=\"1\">\n");
			fprintf($html_fp,"<td align=\"center\" valign=\"bottom\">");
			fprintf($html_fp,"</td>\n");
			for($i=$CriterionBase;$i<$valCount;$i++)
			{
				fprintf($html_fp,"<td align=\"center\" width=20>%d</td>\n",$valPerCriterion[$i][0]);
			}
			fprintf($html_fp,"</tr>\n");
	}	

	if($isHour)
	{
		fprintf($html_fp,"<tr>\n");
		fprintf($html_fp,"<td align=\"center\" valign=\"bottom\">&nbsp</td>\n");
		for($i=$CriterionBase;$i<$valCount;$i++)
		{
			fprintf($html_fp,"<td align=\"center\" ><img src=\"./img/hr%d.png\" width=\"10\"/></td>\n",($i%12)+1);
		}
		fprintf($html_fp,"</tr>\n");
	}	
	
	for($j=0;$j<count($CriterionArray);$j++)
	{
		if($CriterionArray[$j]!="")
		{
			fprintf($html_fp,"<tr>\n");
			fprintf($html_fp,"<td align=\"center\" valign=\"bottom\">%s</td>\n",$CriterionArray[$j]);
			for($i=$CriterionBase;$i<$valCount;$i++)
			{
				if(!$CriterionNotNull || $valPerCriterion[$i][0]!=0)
					fprintf($html_fp,"<td width=\"14\" align=\"center\" >%s</td>\n",$valPerCriterion[$i][$j+1]);
				else
					fprintf($html_fp,"<td width=\"14\" align=\"center\" > &nbsp </td>\n");
			}
			fprintf($html_fp,"</tr>\n");
		}
	}	

	fprintf($html_fp,"</table>\n");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
}

function drawGraphInnerHorizontal($html_fp,$valPerCriterion,$isHour,$CriterionBase,$valCount,$header,$ValueName,$CriterionArray,$CriterionNotNull)
{
	$maxTotal = 0;
	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		if($valPerCriterion[$i][0]>$maxTotal)
			$maxTotal = $valPerCriterion[$i][0];
	}

	global $table_news_bgcolor_color;
	global $table_news_head_color;
	global $table_news_row_color;
	

	fprintf($html_fp,"<table width=\"\" border=\"0\" cellpadding=\"0\" cellspacing=\"1\"  bgcolor=\"%s\">\n",$table_news_bgcolor_color);
	fprintf($html_fp,"<th bgcolor=\"%s\" align=\"center\" >\n",$table_news_head_color);
	fprintf($html_fp,"%s\n",$header);
	fprintf($html_fp,"</th>\n");
	fprintf($html_fp,"<tr>\n");
	fprintf($html_fp,"<td>\n");
	
	fprintf($html_fp,"<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"3\" bgcolor=\"%s\">\n",$table_news_row_color);
	for($i=$CriterionBase;$i<$valCount;$i++)
	{
		fprintf($html_fp,"<tr>\n");
		for($j=0;$j<count($CriterionArray);$j++)
		{
			fprintf($html_fp,"<td>\n");
			if($CriterionArray[$j]!="")
			{
				if(!$CriterionNotNull || $valPerCriterion[$i][0]!=0)
					fprintf($html_fp,"%s\n",$valPerCriterion[$i][$j+1]);
				else
					fprintf($html_fp," &nbsp \n");
			}
			fprintf($html_fp,"</td>\n");

			
		}
		if($isHour)
		{
			fprintf($html_fp,"<td>\n");
			fprintf($html_fp,"<img src=\"./img/hr%d.png\" width=\"10\"/>\n",($i%12)+1);
			fprintf($html_fp,"</td>\n");
		}	

		fprintf($html_fp,"<td align=\"right\">\n");
		fprintf($html_fp,"%d\n",$valPerCriterion[$i][0]);
		fprintf($html_fp,"</td>\n");
		
		fprintf($html_fp,"<td>\n");
		$v = $valPerCriterion[$i][0];
		fprintf($html_fp,"<img align=\"bottom\" src=\"./img/vp.png\" height=\"6\" width=\"%d\" alt='value : %d' title='value : %d' />\n",100*$v/$maxTotal,$v,$v);
		fprintf($html_fp,"</td>\n");


		fprintf($html_fp,"</tr>\n");
	}	
	
	fprintf($html_fp,"</table>\n");
	
	fprintf($html_fp,"</td>\n");
	fprintf($html_fp,"</tr>\n");
	fprintf($html_fp,"</table>\n");
}

?>