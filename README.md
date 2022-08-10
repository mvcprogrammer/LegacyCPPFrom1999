# LegacyCPPFrom1999

One of my first programming jobs was to create a daytrading platform for onsite taders with a company called "PointDirex".

I had no experience, and had only studied a C++ book from WROX titled "Begining C++ by Ivor Horton". But I was determined.

So I ordered the hardware, ran the ethernet cables, configured the hubs, figured out the difference between TCP and UPD and wrote a software program that fed real time stock data to desktop UI that incuded charts, graphs and had to ability to buy, sell and short stock on the NASDAQ.

Back then, messages were sent using fixed length strings and fields separated by the pipe | - there was no XML or JSON, just buffers full of stock prices blasting 600 messagges per second, which had to be sorted and trasmitted to what ended up about 60 traders.

These files were used to help make this happen and is some of the first code I ever wrote.

These files are still currently used on a MLS patform the interfaces with StellarMLS, although it's being replaced with an API connection using c#. Sockets have been replaced with HTTPClients, and this code is now being sunsetted.

