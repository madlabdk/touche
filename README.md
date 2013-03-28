touche
======

Touché implementation w/ Arduino and PureData - Max/MSP

<strong>INTRODUCTION</strong>

Touché is a capacitive-sensing technology developed by Walt Disney Research, which is able to provide touch and gesture detection to virtually any kind of object or surface. Unlike common capacitive sensing, which detects the capacitive load of a resonant circuit over a single frequency, Touché uses a variable frequency oscillator, taking many readings at many different frequencies. Outside world activity with the object can be then inferred by the resulting data profile.

The original paper is <a href="http://www.disneyresearch.com/research/projects/touchechi2012.pdf">here</a>.

<strong>CREDITS</strong>

<a href="http://dzlsevilgeniuslair.blogspot.se/">DZL </a>and <a href="http://hobye.dk">Mads Hobye</a> are 2 awesome guys @ <a href="http://www.illutron.dk/">Illutron</a> which figured out a very clever way to implement this neat device using an Arduino and a bunch of very cheap and common passive components. They were kind enough to share their effort to the world <a href="http://dzlsevilgeniuslair.blogspot.it/2012/05/arduino-do-touche-dance.html">here</a> and <a href="http://arduino.cc/blog/2012/06/01/touche-with-arduino/">here</a>, and to make this wonderful <a href="http://www.instructables.com/id/Touche-for-Arduino-Advanced-touch-sensing/">instructable</a>. So we decided to try and build the little thingie on our own and to interface it with two environments we love: <a href="http://puredata.info">PureData</a> and <a href="http://www.cycling74.com">Max/MSP</a>.

<strong>HARDWARE</strong>

Components list for the touché board:
<ul>
	<li>Resistors: 10k, 1M, 3,3k</li>
	<li>Capacitors: 100pf, 10nf</li>
	<li>Diode: 1N4148</li>
	<li>Coil / inductor: 10mH</li>
</ul>
We decided to build a separate board with standard PCB instead of an Arduino shield, to possibly make it portable on other microcontrollers or different layouts of the Arduino board (micro, mega and so on). These are the schematics:

<a href="http://madlabdk.files.wordpress.com/2013/03/schematics.jpg"><img class="alignnone size-medium wp-image-22" alt="schematics" src="http://madlabdk.files.wordpress.com/2013/03/schematics.jpg?w=300" width="300" height="213" /></a>

and this is what we built:

<a href="http://madlabdk.files.wordpress.com/2013/03/top.jpg"><img class="alignnone size-medium wp-image-23" alt="top" src="http://madlabdk.files.wordpress.com/2013/03/top.jpg?w=300" width="300" height="225" /></a>

<a href="http://madlabdk.files.wordpress.com/2013/03/bottom1.jpg"><img class="alignnone size-medium wp-image-26" alt="bottom" src="http://madlabdk.files.wordpress.com/2013/03/bottom1.jpg?w=300" width="300" height="225" /></a>

<a href="http://madlabdk.files.wordpress.com/2013/03/overlay.jpg"><img class="alignnone size-medium wp-image-25" alt="overlay" src="http://madlabdk.files.wordpress.com/2013/03/overlay.jpg?w=300" width="300" height="225" /></a>

The RED wire on the left is connected to pin 9, which will act as a variable frequency oscillator. The YELLOW wire carries the output signal and is connected to analog pin 0. The BLUE wire on the left is connected to ground. On the right side, the RED wire is attached to the object, while the BLUE wire may be connected to ground or to another object, depending on the applications.

The original implementation by Disney uses a very precise sine wave generator, while we have to live with a much less fancy square wave with lots of unwanted harmonics. As DZL found out this is not a major problem, since the signal picked AFTER the coil is stable enough to be used and processed.

<strong>SOFTWARE</strong>

The ATMEL processor of the Arduino provides three programmable hardware clocks, which can operate at the frequencies needed by Touché and thus provide the needed oscillating signal.  Programming the hardware clocks of an Arduino is a bit trickier than writing your average sketch because we have to mess directly with the ATMEL registers. Please look at <a href="http://arduinodiy.wordpress.com/2012/02/28/timer-interrupts/">this</a> very well made tutorial if you need further information. Basically what we did was to setup CTC mode for Timer1 and increment the compare register inside a for loop, in order to progressively swipe from the highest generable frequency (8MHz) to the lower ones.

The original project from DZL and Mads Hobye sends the digitized measures coming from analog pin 0 through the serial port in binary format. We decided to make things a little less efficient but more human (and patcher) readable using ASCII encoding. Each line of data is made by the frequency index, a space character, the measured value for that frequency and a newline character.

PureData and Max/MSP patches are very similar between them: they read the lines of data coming from the serial port, store the values into an array and find the index of the greatest value in the array. To detect a new gesture, the user has to perform it on the object and record the peak index as a reference. The closest reference to the new peak will then correspond to the detected gesture.

We decided to add an exponential moving average on the signal readings, in order to further reduce noise and improve detection consistency. For each frequency index <em>i</em>, the new value <em>y</em> at time <em>n</em> is calculated as follows:

<em>y[i,n] = a * y[i,n-1] + (1 - a) * x[i,n]</em>

where <em>x[i,n]</em> is the current reading and a is a factor ranging from 0 to 1. Little values of <em>a</em> mean little smoothing and fast updates, while great values of <em>a</em> mean lots of smoothing but very slow updates.

Both the detection and the smoothing algorithm are very simple yet cumbersome to write inside of a patcher language such as PureData or Max/MSP. We found it much easier to do all the data processing inside of the Arduino, outputting just index and value of the maximum for each iteration. It becomes impossible to plot the fancy graph, but the patches become a lot cleaner and we don't clog the slow serial Arduino to PC communication with lots of unwanted data.

You can download the code for both versions (<em>touche_graph</em> and <em>touche_peak</em>, respectively<em>)</em> on our github repository.

Enjoy!
