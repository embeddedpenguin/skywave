~~PREFACE
If you're going to run the demo, simply download the .rar, unzip and run the exe. Read the instructions on how to use the demo below.

If you're going to want to edit the code to add more graphs and rename things, you'll need to install Qt. After you install Qt, download the project files from here:

https://github.com/adragott/skywave (Ignore the README on github)

NOTE: I highly recommend just using the demo if you don't have Qt already installed and aren't proficient with C++. Qt takes a super long time to install and can randomly cut out during the install, making you hate life.

Once you've extracted and saved the folder to where ever you want, open the .pro file and pick your compiler kit. Hit configure project and you're done. You'll have to figure things out for the Qt instructions because theres too much to explain here without a full tutorial. All you really need to know is (inside of your main.cpp) :

	groundStation.CreateNewPlot("PlotNameHere"); //this creates a new plot

	//this creates a plot with x and y axis titles
	groundStation.CreateNewPlot("PlotNameHere", "XValueTitleHere", "YValueTitleHere"); 
	


Make sure these are called after the declaration and initialization of groundStation and before groundStation.show();



~~DEMO INSTRUCTIONS:
The demo works by taking in whatever serial data you're sending from your MCU and automatically graphing it, regardless of how many values you send it. The demo only has 4 graphs, so I'd recommend sending 4 values. You can send more or less and nothing will break, though.

From Atmel, program your MCU to send a printf statement in the following format:

	printf("%d,%d,%d,%d\n", val1, val2, val3, val4);

Simply separate each value with a comma and end the string with a '\n' character.

Note that the order of values sent will be the same as the order of values graphed. So val1 will be first graphed, val2 second, and so on.

If the groundstation doesn't connect and throws you an error saying something like "Device not found" go into the settings and change your comport to the one your MCU is using. If it's not there, your MCU isn't plugged in correctly.

If the groundstation does connect but it doesn't log or graph any values, it's likely due to the baudrate being wrong. Change it in the settings.

NOTE: log.csv files will be produced in the folder containing the .exe.