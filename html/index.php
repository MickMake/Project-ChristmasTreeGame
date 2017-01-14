<!DOCTYPE html>
<html>
<head>
<style>
body {
	text-align: left;
	color: #FFFF80;
	font-weight: normal;
	font-size: 1.1em;
}
h2 {
	color: #FFFF80;
	text-align: center;
}
a {
	color: #FFFF80;
	font-weight: bold;
}
td {
	vertical-align: center;
	text-align: center;
}
p.sansserif {
    font-family: Arial, Helvetica, sans-serif;
}
</style>
</head>
<body background="background3.jpg">
<script src="https://apis.google.com/js/platform.js"></script>
<script>
function Fire($elf)
{
	// document.getElementById("Result").innerHTML = "Fire!";
	// document.getElementById("Fire").innerHTML='<object type="type/html" data="throw.php?elf=' + $elf + '" ></object>';
	var req = new XMLHttpRequest();
	req.open('GET', 'throw.php?elf=' + $elf, false);
	req.send(null);
}
</script>

<table>
<tr>
<td style="text-align: left; background-color: rgba(0, 0, 0, 0.60)">

<p>
<a href="https://youtube.com/c/MickMakes"><img src="banner.png" align="middle"></a>
</p>
<h2>Knock Santa Off His Sleigh</h2>

<table>
<tr>
<td>
<div class="g-ytsubscribe" align="center" data-channelid="UC7GMT3ohvYEAJFDenzj9EMQ" data-layout="full" data-count="default"></div>
<a href="https://www.youtube.com/channel/UC7GMT3ohvYEAJFDenzj9EMQ?sub_confirmation=1">Subscribe to throw a snowball!</a>
</td>
<td>
<a href="http://patreon.com/mickmake">Support me on<br /><img src="http://santa.mickmake.com/PatreonLong2.png" width="240" height="60"></a>
</td>
</tr>

<td>
<p style="text-align: right"> <button onclick="Fire(0)">Elf 1</button> <button onclick="Fire(2)">Elf 2</button> <button onclick="Fire(4)">Elf 3</button> <button onclick="Fire(6)">Elf 4</button> <button onclick="Fire(8)">Elf 5</button>
------
<button onclick="Fire(1)">Elf 6</button> <button onclick="Fire(3)">Elf 7</button> <button onclick="Fire(5)">Elf 8</button> <button onclick="Fire(7)">Elf 9</button> <button onclick="Fire(9)">Elf 10</button> </p>
</td>
<td>
</td>
</tr>

<tr>
<td> <iframe width="960" height="540" style="float:left" src="https://www.youtube.com/embed/AAlQtP18UAw?autoplay=1" frameborder="2" allowfullscreen></iframe> </td>

<td>
<p>This is a live YouTube stream of my Christmas tree.
<br />
Stay tuned <a href="https://youtube.com/c/MickMakes">on my channel</a> when I publish the tutorial on how I did this.<br />
<br />
<span style="color:red">Santa</span> is running wild again, and it's the job of the <span style="color:cyan">Blue Elves</span> to knock him off his sleigh.
See if you can get an <span style="color:cyan">Elf</span> to hit <span style="color:red">Santa</span> with a <span style="color:white">snowball</span>.
Just click on any button to throw one.
When you hit <span style="color:red">Santa</span> it'll <span style="color:orange">make a mark.</span><br />
<br />
Since <span style="color:cyan">Elf</span> telemetry is so bad, there's a good <b style="color:yellow">10 second delay</b> between telling an <span style="color:cyan">Elf</span> to throw a snowball and them actually doing it! <b style="color:yellow">(You can blame YouTube for that.)</b> See if you can judge the delay.<br />
</p>
<br />
<br />
</td>
</tr>

<tr>
<td>
<p id="Result"></p>
</td>
<td>
<div id ="Fire"> </div>
</td>
</tr>
</table>

</table>

</body>
</html>
