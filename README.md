# temperature-ds
Use NintendoDS as a WiFi Temperature station

<p align="center">
<img src="https://raw.githubusercontent.com/dbeef/temperature-ds/master/readme/plot_small.png" alt="12 sec transmission"
 width="650" height="400">

<h2 align="center">Dependencies</h2>
<ul>
 <li>linux</li>
<li>DevkitPro, easiest way to download it is through pacman: https://devkitpro.org/wiki/Getting_Started</li>
</ul>

<h2 align="center">Usage</h2>

<ul>
 
 <li> create hotspot on your laptop </li>
 <li> run .nds file on your NDS </li>
 <li> connect to your hotspot on your NDS via the .nds file </li>
 <li> run the python script: desktop_client.py </li> 
 
</ul>

<h2 align="center">How it works</h2>

<ul>
<li>NDS opens port 8080, python script connects PC to it, then:</li>
  <ul>
    <li> NDS receives ACK character from NDS</li>  
    <li> NDS measures temperature,</li>  
    <li> sends its value </li>
    <li> python script appends stats array </li>
    <li> python script waits 1s </li>
    <li> repeat</li>  
    </ul>
</ul>

<h2 align="center">How to make WIFI hotspot that NDS can connect (it's not that obvious):</h2>

<ul>
<li>in Ubuntu open "Network Connections" and click "add new"</li>
<li>go to the "Wi-Fi security" - NDS supports only WEP security (so remember, it's dangerous to beacon that poorly secured network all the time, use this hotspot only when playing with your NDS) - select "WEP 40/128 bit key (Hex or ASCII)" </li>
 <li> select some random 5 character (only digits) password </li>
 <li> in "Wi-Fi" tab select "Mode" to "Hotspot" </li>
 <li> save it </li>
 <li> open "Network" </li>
 <li> click "Connect to to a hidden network" </li>
 <li> select network that you've just created (by default its name will be "hotspot")
 <li> you're done, if you were previously connected with another network via wifi you'll be disconnected</li>
 <li> now you can connect to this network through your NDS </li>
</ul>

<h2 align="center">Performance:</h2>
<p> As I figured out, NDS temperature sensors just aren't made for this purpose, lol. Temperature oscillates +-1C almost every second, though it still detects temperature changes, so If you smooth the output plot it may be accurate enough. Here's a plot of 5 minutes of measurment every 1 second:</p>


<p align="center">
<img src="https://raw.githubusercontent.com/dbeef/temperature-ds/master/readme/plot_big.png" alt="Efficiency"
 width="650" height="400">

<p align="center"> I decided to smooth it by linear interpolation, so the same data but transformed look like this:
<img src="https://raw.githubusercontent.com/dbeef/temperature-ds/master/readme/plot_interpolation.png" alt="Efficiency"
 width="650" height="400">

<h2 align="center">Acknowledgments:</h2>
<ul> I used devkitPro libnds examples of interprocessor (ARM7<->ARM9) transmission for probing temperature, also modified their example of connecting to the access point  </ul>
