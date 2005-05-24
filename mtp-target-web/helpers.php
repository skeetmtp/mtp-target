<?php

require_once("stat_function.php");
require_once("stat_game.php");

function boxBeginFp($html_fp,$expand)
{
	if($expand)
		fprintf($html_fp,"<table width=\"100%%\" class=\"stat\">");
	else
		fprintf($html_fp,"<table class=\"stat\">");
	fprintf($html_fp,"<tr><td>");
}

function boxEndFp($html_fp)
{
	fprintf($html_fp,"</td></tr></table>");
}

function boxBegin($expand)
{
	if($expand)
		printf("<table width=\"100%%\" class=\"stat\">");
	else
		printf("<table class=\"stat\">");
	echo "<tr><td>";
}

function boxEnd()
{
	echo "</td></tr></table>";
}

?>
