<?php
if (isset($_GET['elf']))
{
	$fp = fsockopen("TARGET_SERVER", 8080, $errno, $errstr, 30);
	if (!$fp)
	{
		echo "$errstr ($errno)<br />\n";
	}
	else
	{
		fwrite($fp, $_GET['elf']);
		fclose($fp);
		// usleep(4000);
		flush();
	}
}
?>

