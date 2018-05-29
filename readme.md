project "hockey" readme (wordwrap) 
=========================
(went a bit overboard on this, you might want to skip to the end :)

	Intro
	How to play
	Gameplay features
	Technical features
	Todo List
	Bug list
	Finally... the proper readme bits




Intro
=========================
(in case your not taking this from the gbadev.org competition entries...) this is my entry for the gbadev.org gba competition [:)]. its my second project coding for the gba (the first being "ferk" -a basic gta clone- if you havent heard of it) and i ended up spending a lot of time on it, although its one of my first projects i didnt get tired of; there always seemed like another natural feature (more hockey features) or general cool features (the scoreboard for example) to add in. and the results were pretty impressive, and pretty quick to implement. more praise for the gba, no ghastly memory locking or driver crap to initialise.

the original idea was to make a basic clone of nhlpa (93 i think, the first nhlpa title, as apposed to ea hockey, or the one that was on the NES, i played the mega drive/genisis version myself) seeing as [at the time] there werent any plans on nintendo/someone on making a hockey game for the GBA (or porting one as it seems theyre now doing, yes, nhlpa is on its way :D)

Unfortunetly due to real life stuff and general laziness, there are a number of features i never had time to implement. (theyre in the todo list) but despite that, the game is still playable to a large extent, theres just a lack of "game rules" (periods, time outs, fouls, end of games etc) and no front end unfortunetly. i could of knocked one up, but i really wouldnt like it, and would probably take up too much valuable time. not to mention the lack of sound, but i really didnt have the time to look into it.


a quick calculation, estimates the time i spent on this was, 100 hours. (i really cant be bothered to work out if this is accurate, but it seems right) we'll see if this wasnt wasted time :]. im hoping the gbadev community at least will get a kick out of the game, and mess with the source, thats why its given out.




How to play
=========================
When the game starts (or after a goal) the players will all go to line up at the centre of the rink from there youll get control of a player. anyone whos played nhlpa (the early ones at least) will find these controls familiar(blatantly copied some may say). 

the main objective of the game, obviously, is to get the puck into the other teams goal. either by shooting it in, or "walking" the puck into the goal (holding on to it and running into the open of the goal, im sure i dont need to explain that). after you score, the players will all line up again, and the scoreboard changes back to a countdown till play starts again (hold a or b to speed up the countdown)

to get the puck off a player, the only way at the moment (tackling is in the todo :/) is to barge into him(easier with the control below), he will then drop the puck, and its most likely youll grab it or someone else will straight away.

the player under your control has a red rink around him. now, there are 2 sort of stages to controlling your player;

	- with puck:
	- A - this is the shoot. hold it down, and the player will swing his stick, up to a maximum point (you can release it earlier, and the shot will be lighter and finish quicker). the shot aims directly at the goal. this would make sense, if the other features were implemented, like goalies, or some sort of inaccuarcy rating.
	- B - Passing! another planned feature was going to go in here, but unfortunetly, we're just stuck with the usual passes to the closest player on your team. the plan was to add passing to the closest player in the direction your facing. making a more, user-choice orientation, which would be a lot less annoying for the player.

	- without puck:
	- A - This gives you a little speed burst, very useful for getting the puck from an enemy, like a barge.
	- B - This changes the selected player to the one on your team closest to the puck.



Gameplay features
=========================
There are probably way too many features to mention here (stuff you wouldnt think of, just because it plain works)

	- AI - unfortunetly, i added basic ai as soon as i added extra players, with some basic ai. if the players havent got the puck, and the person with the puck (if any) isnt on their team, they head for the puck. if they get it, they then run for the goal. its pretty simple, but works suprisingly well, and at first they seem a bit aggressive, but after a while youll see how to beat them quite easily. again, one of the todo's i had planned was to get the players to go into formations if someone on their team had the puck, or one of the opposing teams has the puck (and assuming to head to the goal). implementing passing and shooting is easy enough too, but due to time limits, it had to be left out. still, the ai is there, and provides basic opposition. (you wont win if you just leave them to play :)

	- "stats" - press select to toggle the in-game stats (debug stuff really). all the things are: fps (frames per second), mode (internal game mode, in play, idle, init, line up etc), countdown number, default control, the tile allocation per bg, and the current x/y scroll. press select again to turn it off

	- scoreboard - a custom designed system to make some fancy graphics (in place of some good sound). animations are actually a set of tiles (block of 4 lights, with on/off combinations) generated with a tool of mine which just makes up the numbers. you then just drop it, and assign the animation where needed in the source. (the tool wont be distributed with the source, so if you want it, i can send it). the scoreboard has also been designed to handle basic numbers, so i can add a constant display of the score, and the countdown.

	- crowd - the last main feature i put in and completed, is a fully updated, animated crowd. the crowd layout is based on their direction (set in the rink's floor tile map) and a code for their crowd number (1 red, 2 empty or 3 blue). as they appear on screen, they get updated, and as the game loops, it picks a non-linear order of rows to update at a time, giving a non-uniform update (like a crowd). there was a mexican wave (in fact its still there and working), but to get it working properly, the WHOLE crowd data would have to get updated constantly, and theres not an effecient enough way of doing that, so instead the crowd seems to randomly update (get out of their seats and cheer). again, its a feature you probably wouldnt recognise or notice, but its there, and its cool :]

	- hockey sticks - to catch the puck, you have to collide it with the end of the stick, the same with moving it about, the puck gets "stuck" onto the edge of your stick (this actually causes a bug, see the bug list). but looks a lot more realistic. i can also have players without sticks, and save space on the player sprites, 16x8 instead of 16x16 or even 32x32 for the same amount of data.

	- configuaration - because of the very flexible way ive coded it, there are a whole load of options you can quickly change, the player's uniforms (it jsut needs different tilesets, and a different init line). to change the size of the rink, you need to edit 2 #define's and update the tilesets for the rink floor, screen and rink tiles. there is a lot thats easy to change, i just havent made a variable version (its all defines). you can turn off the crowd, or turn off the scoreboard etc etc




Technical features
=========================
techniccly this game is packed with techniques ive developed overtime, everything of course stemming from the pern project tutorials. heres a quick list of some of the tech features ive developed and used in the game

	- dynamic tile loading - when you scroll further than 32x32 tiles in the memory, the system reloads the rows and columns that are now not visible on-screen so any slight change in scrolling shows the new tiles in the map (in this case the ice rink). this whole system then requires sprites to be placed at certain positions on screen (rather than their actual coords) so that brings in a camera too(improved in the "zombie" project)

	- dynamic tile allocation system - when you init a level or a menu screen etc, you only have to load the tiles you require on whichever background they will apear on, then when theyre needed in-game you use a basic define (it just looks into an array) that will return the tile number (byte) where that tileset starts. for example BG_TILE(HUD_BG,FONT_TILES) returns the start of the alphabet tiles for the HUD_BG (background 0). assuming they were loaded in before this is accessed (in hockeys case, in init_game() ) then any font tile will be from that returned value, rather than a hard-coded tile start. This means by chaning only 2 lines in the code, we can change the outfits for the hockey players, we just load a different tileset and set the hockey players' tile set to this new one. (unfortunetly this is one of the features i didnt have time to add in, see below). but can be done almost straight away. this means you can also switch tilesets for the rink, to quickly change different team emblems on the ice, or to change the crowd members (crowd will be coming up later)

	- Animated sprites - a basic system of cycling through tiles in a tilset for a set number of frames, skipping frames (for bigger sprites), stopping animations, looping, time delays. my whole sprite system needs rewriting really, its all very ineffecieant, and using (gasps) bitfield casting (if there were ever two words that shouldnt go together it could easily be those two). but it works.

	- player control - nothing special really, but its an easy way to switch between modes, from player control (controlled by gba input), ai control (a bot) and a no brained bot (tell a bot to go here, and itll just move towards it). but the fact you can quickly change these flags per-player, maybe even making a instant replay control. (or maybe not :P, but im sure its possible)

	- mask on mask collision checking - everything has an assigned collision mask. tiles on the rink, the goals, the players sprites. ill start off by explaining that movement in this particular game is governed by an x and y velocity (xspeed and yspeed). this means, pushing "down" increases the yspeed, (every time the player is updated, the very technical formula, x = x+xspeed (same for y) is used to move the player around). this makes "gliding" on ice very easy. now, as you collide with a tile (or other sprite or such an object) the code captures the 2 pixels that collide on the mask. the numbers on the mask are arranged in a bit form, 1 is north, 2 south, 4 east, so, north east is 5(bit1|bit4). now, the system ive used is that, if you hit a north collision bit, your yspeed will move upwards (to make a sort of bounce upwards). the amount you "bounce" is based on your speed, the speed of the object youve collided with, and numerous other factors. the long and short of it is, it works, its quite fast, and it works. and i can apply the same functions to different usages and objects easily.





Todo List
=========================
lots of things i wanted to do, but really didnt have the time.

	- Improved ai - formations, tactics, passing and shooting all needed :]
	
	- Menus - everyone needs menus. to change player strips, configure ai tactics perhaps

	- smoother turning - pressing down after facing up, the player currently instantly turns downwards, the player *should* turn around to face below

	- better passing - passing at the moment just goes straight to the player closest. it needs to pass to the most-accurate player. a trade off between direction and closeness to a player on the same team

	- goalies! - its too easy to score at the moment, especially if you just shoot, itll go in. we need someone to get in the way :]

	- face off's - putting a bit more of the proper hockey game rules in will be good.

	- tackling - its ready to be put in, i just need some variables/formulas to allow the player to extend his stick out and check if its overlap a player, and then steal the puck (if applicable)






Bug list
=========================

	- every so often (not very common at all, and so almost impossible to track down...) sometimes the rink reloads itself in the wrong place. i may be writing over some other memory, or it might be a 1 -in-1000 chance exception. the symptons are just that youll be able to see the bottom of the bottom goal in the middle of the rink. which wont get reloaded until you scroll right to the top

	- because i moved the puck placement to the end of the stick, its actually possible to score in the top goal, but moving behind it, and pushing down

	- oddly enoguh, thats all the major bugs sorted :]. suprising what you can achieve without a proper source debugger :]







Finally... the proper readme bits
==============
any source i put up on my site is free. no charges. gratis. if you make a game or decent demo out of it, id like to know, purely cos im nosy.

if someone wants to expand on the code, add all those things i wanted to do (in the todo list) that i never got around to, knock yourself out! i probbaly have a whole bunch of other things that i didnt add to that list :]

feel free to ask about any specific parts of my code, or how/why i did this/that. you can ask me to make a game for you, but it aint gonna happen :]

if you want to improve on some of the things in the game (extra scoreboard anims etc) itd probably be best to ask me for the [source of the]tools ive used to make em with.

thanks to anth for his wonderful mapping tool, 
thanks to everyone else on #gbadev (great chaps, even if communism gets in the way)
thanks to simonb and co (gbadev.org) for the prizes (you just know theyre coming my way :) as well as for the collection of links and docs (an invaluble site)
thanks to nintendo. for the n64.
thanks to the classic EA for nhlpa(and many other fine games of the 16 bit era)

graham@ferk.co.uk <---- me
www.ferk.co.uk <---- check back for updates on my sources, or any new demos/sources etc
www.gbadev.org <---- go here, daily. any time you can
#gbadev on efnet <---- go here just as often :)



--fatgraham






