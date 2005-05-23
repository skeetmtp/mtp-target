<?php

if(CUser::instance()->admin())
{
	echo '<hr /><div id="footer">';
	echo $var_bench;
	echo 'Page generated in <b>'.bench_end().' seconds</b>';
}
echo '</body></html>';
ob_end_flush();

?>
