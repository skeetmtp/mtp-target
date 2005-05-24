<?php
require_once("stat_function.php");

		$limit_range=10;
	if(!isset($limit_base) || !validNumber($limit_base)) 
		$limit_base=0;
		
	$cacheFileName = $cache_dir."/stat_user_texture_".$limit_base.".html";
	if(isCacheFileUpToDate($cacheFileName))
	{
		require_once($cacheFileName);
		return;
	}
	$html_fp = fopen($cacheFileName, "wt");



	$thisPage = sprintf("?page=stat_user_texture");
	
	
	$requete = "SELECT count(*) as c,user.Texture FROM user WHERE user.Texture!='blue' GROUP BY user.Texture ORDER by c DESC LIMIT $limit_base,$limit_range;";
	$result = exec_game_db_requete($requete);
	$line_count = 1;
	while ($line = mysql_fetch_array($result))
	{
		$userTextureUrl = userTextureBame2Url($line[1]);
		fprintf($html_fp,"%s used %d times <br/><br/><br/>",$line[1],$line[0]);
		fprintf($html_fp,"<img src=\"%s\" ALT=\"user texture : %s\"><br/><br/>\n",$userTextureUrl,$userTextureUrl);
	}	
	
	$previous_base = $limit_base-$limit_range;
	if($previous_base<0)
		$previous_base=0;
	fprintf($html_fp,"<a href=\"%s&limit_base=%d\">&lt;&lt;prev</a>",$thisPage,$previous_base);
	fprintf($html_fp,"&nbsp;&nbsp;&nbsp;&nbsp;");
	fprintf($html_fp,"<a href=\"%s&limit_base=%d\">next&gt;&gt;</a>",$thisPage,$limit_base+$limit_range);
	

	fclose($html_fp);	  
	require_once($cacheFileName);	


	
?>