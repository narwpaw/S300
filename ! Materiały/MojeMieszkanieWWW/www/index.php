<?
$MyPassword='';
function stripslashes_deep($value)
{
$value = is_array($value) ?
array_map('stripslashes_deep', $value) :
stripslashes($value);
return $value;
}
$has=stripslashes_deep($_POST['has']);
if (($has==$MyPassword) || ($MyPassword=='')) $pass='ok';
?>
<html>
<head>
<meta http-equiv="Content-type" content="text/html; charset=iso-8859-2" />
<title>CEURON Smart home</title>
<meta name="description" content="Ceuron smart home - user site" />
<meta name="keywords" content="inteligentny dom, automatyka budynku, automatyka, smart home,"/>
<meta name="classification" content="global,all" />
<meta name="robots" content="all,index,follow" />
<meta name="revisit-after" content="2 days"/>
<meta name="author" content="Ceuron">
<meta name="description" content="" />
<link rel="Shortcut icon" href="images/ikona.ico" />
<link rel="stylesheet" href="style.css" type="text/css" />

<script type="text/javascript" src="prototype.js"></script>
<script type="text/javascript" src="jquery.js"></script>

<?
if ($pass=='ok') echo'
<script>
var Vold = new Array();
var str;
var idx=0;
var V_c99;
var V_a01;
var V_a05;
var V_b04;
var V_b06;
var V_b07;
var V_b13;
var V_b14;
var V_b20;
var V_a06;
var V_c13;
var V_d03;
function PeriodFunction(){

        idx++;
       $.ajax({
	       cache: false,
	       url : "di3100137.csv",
	       success : function (data) {
			var arr = data.split('."'".'\n'."'".');
			str=arr;
		}
	});

        var Res='."'".'Recive:'."'".'+str;
        var V;
        var V2;
        var el1;
        var PZM=-1;
        var el = document.getElementById('."'".'Box0'."'".');
        el.innerHTML = '."'".'SERVER CONNECTIONS: '."'".'+idx;

        el1 = document.getElementById('."'".'Box6'."'".');
        PZM=Res.indexOf('."'".'c99'."'".');
        if (PZM>=0){V="";i=4;while ((i<9) && (Res.charAt(PZM+i)!=0xd)){V=V+Res.charAt(PZM+i);i++;}}else{V='."'".'0.0'."'".';}
        el1.innerHTML ='."'".' '."'".' +V+'."'".''."'".';
        el1 = document.getElementById('."'".'Box7'."'".');
        PZM=Res.indexOf('."'".'a01'."'".');
        if (PZM>=0){V="";i=4;while ((i<8) && (Res.charAt(PZM+i)!=0xd)){V=V+Res.charAt(PZM+i);i++;}}else{V='."'".'0.0'."'".';}
        el1.innerHTML ='."'".' '."'".' +V+'."'".' C'."'".';
        el1 = document.getElementById('."'".'Box8'."'".');
        PZM=Res.indexOf('."'".'a05'."'".');
        if (PZM>=0){V="";i=4;while ((i<8) && (Res.charAt(PZM+i)!=0xd)){V=V+Res.charAt(PZM+i);i++;}}else{V='."'".'0.0'."'".';}
        el1.innerHTML ='."'".' '."'".' +V+'."'".' %'."'".';
        el1 = document.getElementById('."'".'Box10'."'".');
        PZM=Res.indexOf('."'".'b04'."'".');
        if (PZM>=0) {V=Res.charAt(PZM+4);}else{V='."'".'0'."'".';}
        if (Vold[0]!=V)
        if (V==1)
        {
        el1.style.backgroundImage="url('."'".'bulb_2b.png'."'".')";
        }else{
        el1.style.backgroundImage="url('."'".'bulb_2a.png'."'".')";
        }
        V_b04=V;
        Vold[0]=V;
        el1 = document.getElementById('."'".'Box11'."'".');
        PZM=Res.indexOf('."'".'b06'."'".');
        if (PZM>=0) {V=Res.charAt(PZM+4);}else{V='."'".'0'."'".';}
        PZM=Res.indexOf('."'".'b07'."'".');
        if (PZM>=0) {V2=Res.charAt(PZM+4);}else{V2='."'".'0'."'".';}
        if ((Vold[1]!=V) || (Vold[2]!=V2))
        if (V==1)
        {
        el1.style.backgroundImage="url('."'".'blind_2a.png'."'".')";
        }else if (V2==1)
        {
        el1.style.backgroundImage="url('."'".'blind_2c.png'."'".')";
        }else{
        el1.style.backgroundImage="url('."'".'blind_2b.png'."'".')";
        }
        V_b06=V;
        V_b07=V2;
        Vold[1]=V;
        Vold[2]=V2;
        el1 = document.getElementById('."'".'Box14'."'".');
        PZM=Res.indexOf('."'".'b13'."'".');
        if (PZM>=0) {V=Res.charAt(PZM+4);}else{V='."'".'0'."'".';}
        PZM=Res.indexOf('."'".'b14'."'".');
        if (PZM>=0) {V2=Res.charAt(PZM+4);}else{V2='."'".'0'."'".';}
        if ((Vold[3]!=V) || (Vold[4]!=V2))
        if (V==1)
        {
        el1.style.backgroundImage="url('."'".'blind_2a.png'."'".')";
        }else if (V2==1)
        {
        el1.style.backgroundImage="url('."'".'blind_2c.png'."'".')";
        }else{
        el1.style.backgroundImage="url('."'".'blind_2b.png'."'".')";
        }
        V_b13=V;
        V_b14=V2;
        Vold[3]=V;
        Vold[4]=V2;
        el1 = document.getElementById('."'".'Box17'."'".');
        PZM=Res.indexOf('."'".'b20'."'".');
        if (PZM>=0) {V=Res.charAt(PZM+4);}else{V='."'".'0'."'".';}
        if (Vold[5]!=V)
        if (V==1)
        {
        el1.style.backgroundImage="url('."'".'bulb_2b.png'."'".')";
        }else{
        el1.style.backgroundImage="url('."'".'bulb_2a.png'."'".')";
        }
        V_b20=V;
        Vold[5]=V;
        el1 = document.getElementById('."'".'Box18'."'".');
        PZM=Res.indexOf('."'".'b20'."'".');
        if (PZM>=0) {V=Res.charAt(PZM+4);}else{V='."'".'0'."'".';}
        if (Vold[6]!=V)
        if (V==1)
        {
        el1.style.backgroundImage="url('."'".'bulb_2b.png'."'".')";
        }else{
        el1.style.backgroundImage="url('."'".'bulb_2a.png'."'".')";
        }
        V_b20=V;
        Vold[6]=V;
        el1 = document.getElementById('."'".'Box20'."'".');
        PZM=Res.indexOf('."'".'a06'."'".');
        if (PZM>=0){V="";i=4;while ((i<8) && (Res.charAt(PZM+i)!=0xd)){V=V+Res.charAt(PZM+i);i++;}}else{V='."'".'0.0'."'".';}
        el1.innerHTML ='."'".' '."'".' +V+'."'".' C'."'".';
        el1 = document.getElementById('."'".'Box21'."'".');
        PZM=Res.indexOf('."'".'c13'."'".');
        if (PZM>=0){V="";i=4;while ((i<8) && (Res.charAt(PZM+i)!=0xd)){V=V+Res.charAt(PZM+i);i++;}}else{V='."'".'0.0'."'".';}
        el1.innerHTML ='."'".' '."'".' +V+'."'".'C'."'".';
        el1 = document.getElementById('."'".'Box24'."'".');
        PZM=Res.indexOf('."'".'d03'."'".');
        if (PZM>=0) {V=Res.charAt(PZM+4);}else{V='."'".'0'."'".';}
        if (Vold[7]!=V)
        if (V==1)
        {
        el1.style.backgroundImage="url('."'".'Auto.JPG'."'".')";
        }else{
        el1.style.backgroundImage="url('."'".'Manual.JPG'."'".')";
        }
        V_d03=V;
        Vold[7]=V;
}

var pe = new PeriodicalExecuter(PeriodFunction, 1);

function Clik(Zm,War){
 $.ajax({
   type: "POST",
   url: "putgate.php",
   data: "NodeID=3100137&Order=2&Content="+Zm+"="+War,
   success: function(msg){
   }
 });

}
function postwith (to,p) { 
  var myForm = document.createElement("form");
  myForm.method="post" ; 
  myForm.action = to ;
  for (var k in p) {
    var myInput = document.createElement("input") ;
    myInput.setAttribute("name", k) ;
    myInput.setAttribute("value", p[k]); 
    myForm.appendChild(myInput) ;
  }
  document.body.appendChild(myForm) ;
  myForm.submit() ; 
  document.body.removeChild(myForm) ;
  }
</script>
';
?>


</head>

<body>

<?
if ($pass=='ok'){ echo'
<div class="external">

  <div id="logo"><a href="www.ceuron.pl"></a></div>

  <div id="MainBox">

  <div id="Wallpaper">

      <div id="Box1"></div>
      <div id="Box2"></div>
      <div id="Box3"></div>
      <div id="Box4"></div>
      <div id="Box5"></div>
      <div id="Box6"> ???.?</div>
      <div id="Box7"> ???.? C</div>
      <div id="Box8"> ???.? %</div>
      <div id="Box9">Nasłonecznienie: </div>
      <a onclick="if (V_b04==0) Clik('."'".'b04'."'".','."'".'1'."'".'); else Clik('."'".'b04'."'".','."'".'0'."'".'); "><div id="Box10"></div></a>
      <div id="Box11"></div>
      <div id="Box12"><input type="button" value="Góra" style="width: 70px" onclick="Clik('."'".'b01'."'".','."'".'1'."'".')"/></div>
      <div id="Box13"><input type="button" value="Dół" style="width: 70px" onclick="Clik('."'".'b02'."'".','."'".'1'."'".')"/></div>
      <div id="Box14"></div>
      <div id="Box15"><input type="button" value="Góra" style="width: 70px" onclick="Clik('."'".'b09'."'".','."'".'1'."'".')"/></div>
      <div id="Box16"><input type="button" value="Dół" style="width: 70px" onclick="Clik('."'".'b11'."'".','."'".'1'."'".')"/></div>
      <a onclick="if (V_b20==0) Clik('."'".'b20'."'".','."'".'1'."'".'); else Clik('."'".'b20'."'".','."'".'0'."'".'); "><div id="Box17"></div></a>
      <a onclick="if (V_b20==0) Clik('."'".'b20'."'".','."'".'1'."'".'); else Clik('."'".'b20'."'".','."'".'0'."'".'); "><div id="Box18"></div></a>
      <div id="Box19">Temperatura zewnętrzna:</div>
      <div id="Box20"> ???.? C</div>
      <div id="Box21"> ???.?C</div>
      <div id="Box22">Termostat</div>
      <div id="Box23"><input type="button" value="Auto +/-" style="width: 70px" onclick="Clik('."'".'d13'."'".','."'".'1'."'".')"/></div>
      <a onclick="if (V_d03==0) Clik('."'".'d03'."'".','."'".'1'."'".'); else Clik('."'".'d03'."'".','."'".'0'."'".'); "><div id="Box24"></div></a>
      <div id="Box25"><input type="button" value="+" style="width: 70px" onclick="Clik('."'".'d23'."'".','."'".'1'."'".')"/></div>
      <div id="Box26"><input type="button" value="-" style="width: 70px" onclick="Clik('."'".'d33'."'".','."'".'1'."'".')"/></div>
      <div id="Box27"></div>
   </div>

  <div id="Box0"></div>

  </div>
</div>
';
  }else{
   echo '<div class="external"><div id="PassBox">
        Insert password<br><br>
        <form action="" method="post">
	      <input type="password" name="has" /><br>
	      <input type="submit" value="OK" />
        </form>
   </div></div>';
  }
?>
</body>

<script>
<?
if ($pass=='ok') echo'
PeriodFunction();
';
?>
</script>
</html>
