<?php
include_once("stat_function.php");
include_once("stat_game.php");


function boxBeginFp($html_fp,$expand)
{
	if($expand)
		fprintf($html_fp,"<table width=\"100%%\"  border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"#666666\">");
	else
		fprintf($html_fp,"<table border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"#666666\">");
	fprintf($html_fp,"<tr>");
	fprintf($html_fp,"<td align=\"left\">");
	fprintf($html_fp,"<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"5\" bgcolor=\"#FFFAEA\">");
	fprintf($html_fp,"<tr>");
	fprintf($html_fp,"<td>");
}

function boxEndFp($html_fp)
{
	fprintf($html_fp,"</td>");
	fprintf($html_fp,"</tr>");
	fprintf($html_fp,"</table>");
	fprintf($html_fp,"</td>");
	fprintf($html_fp,"</tr>");
	fprintf($html_fp,"</table>");
}

function boxBegin($expand)
{
	if($expand)
		printf("<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"#666666\">");
	else
		printf("<table border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"#666666\">");
	printf("<tr>");
	printf("<td align=\"left\">");
	printf("<table width=\"100%%\" border=\"0\" cellpadding=\"0\" cellspacing=\"5\" bgcolor=\"#FFFAEA\">");
	printf("<tr>");
	printf("<td>");
}

function boxEnd()
{
	printf("</td>");
	printf("</tr>");
	printf("</table>");
	printf("</td>");
	printf("</tr>");
	printf("</table>");
}

?>