Yahtzee v1.0
Saturday, 9 December 2006
Written by Damian Coventry
http://damiancoventry.com/



ABOUT
This is an implementation of the classic dice game Yahtzee.

In case you don't know the rules of the game, I've included a file named "rules.html",
which came from http://www.yahtzee.org.uk/rules.html originally.  Yahtzee is pretty simple
to play, and fun, which I'm sure is the reason it's been around for 50 or so years.

With my implementation you may play it with a single player or multiple players on the
same machine by using the "Local Game" button from the Main menu page.  Alternatively,
you may play it over an IP network using the "Network Game" button, also on the Main page.

The game will allow you to play in a window on your desktop, or as a full screen
application which will reduce the screen resolution and take up the entire desktop.

The game will save highscores to disk so that they're retained between invocations of the
application.



NETWORKING
One player must "create" a game, and when they do so a message will be periodically
"broadcast" over the subnet that their computer is on.  The other players will be able to
"see" the game if they're on the same subnet as the game creator.  When they go to the
"Network Game" menu page they may "join" the game by double clicking the desired game's
entry in the main list control, or by typing in that person's IP address and clicking "join.

Before the game starts all players, including the game creator, will be able to exchange
messages with one another.  When everybody has joined the game, the game creator may start
the game by clicking "start".

Players may "alt+tab" out of the game with no adverse affect on the game, other than this
act sending a message to all other players indicating their status as "away".  Likewise, the
players may enter the settings menu during the game and their status indicates them as
"settings".



KNOWN ISSUES TO BE FIXED
- The vertical strokes of the font are sometimes not visible
- Packet loss gets clients out-of-sync quite easily
- The game won't start without a sound card present
- The game won't start without at least one valid IP address present
- Multihomed computers can sometimes end up sending multiple messages to the same game.




UPDATES AND OTHER GAMES
http://damiancoventry.com/



CONTACT
damian.coventry@ipfx.com or damian.coventry@gmail.com
