<html>
<head>
<title>mtp-target</title>
</head>
<body>

	<?php
		function emptyDir($dir)
		{
			if(!file_exists($dir))
			{
				mkdir($dir);
				return;
			}
			
			$handle = opendir($dir);
			if(!handle)
				die("failed to opendir : $dir");
				
			while (false!==($FolderOrFile = readdir($handle)))
			{
				if($FolderOrFile != "." && $FolderOrFile != "..")
				{ 
					if(is_dir("$dir/$FolderOrFile"))
					{ 
						//deldir("$dir/$FolderOrFile");   // recursive
					}
					else
					{ 
						unlink("$dir/$FolderOrFile"); 
					}
				} 
			}
			closedir($handle);
			/*
			if(rmdir($dir))
			{ 
				$success = true; 
			}
			return $success; 
			*/
		} 
		function crcAddDir($fp,$srcDir,$destDir,$validExt)
		{
			$handle = opendir($srcDir);
			if(!handle)
				die("failed to opendir : $srcDir");
			while (false!==($FolderOrFile = readdir($handle)))
			{
				if($FolderOrFile != "." && $FolderOrFile != "..")
				{
					$f = $srcDir . "/" . $FolderOrFile ;
					if(is_dir($f))
					{ 
						crcAddDir($fp,$f,$destDir,$validExt); //recursive
					}
					else
					{ 
						$arr=explode('.',$FolderOrFile);
						$arrSize=count($arr);
						if($arrSize>1)
						{
							$ext = $arr[$arrSize-1];
							if(in_array($ext, $validExt))
							{
								//printf("%s : %s<br>",$f,$ext);
								$sha1 = sha1_file($f);
								printf("%s : %s<br>\n",$FolderOrFile,$sha1);
								fwrite($fp,sprintf("%s\n",$FolderOrFile));
								fwrite($fp,sprintf("%s\n",$sha1));
								$command = "gzip -c ".$f." > ".$destDir.$FolderOrFile.".gz";
								//sleep(1);
								//echo "executing : ".$command."<br>";
								system($command);
							}
						}
					}
				}
			}
			closedir($handle);		
		}

		$ext  = "tga lua dds ps shape xml";
		$exportDirname = "./export/";
		$exportTempDirname = "./export_temp/";
		$crcFilename = "crc.txt";

		$extArray = explode(" ", $ext);
		$fullCrcFilename = $exportTempDirname.$crcFilename;
		emptyDir($exportTempDirname);
		printf("building : '%s'<br>\n",$fullCrcFilename);
		$crcfp = fopen($fullCrcFilename,"wt");
		if(!$crcfp) die("error opening crc file : ".$fullCrcFilename);
		
		crcAddDir($crcfp,"../server/data/",$exportTempDirname,$extArray);
		crcAddDir($crcfp,"../user_texture/",$exportTempDirname,$extArray);
		
		fclose($crcfp);
		emptyDir($exportDirname);
		rmdir($exportDirname);
		rename($exportTempDirname,$exportDirname);
		/*
		$command = "gzip ".$fullCrcFilename;
		echo "executing : ".$command."<br>";
		system($command);
		*/
	?>




</body>
</html>
