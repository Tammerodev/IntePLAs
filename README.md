Libraries : 
SFML
(MODIFIED) TGUI



# Hey. I just want to tell you that this project consist of unsafe code. This is just bad code.
Everything is unsafe, you can get this to segfault whenever you want or dont. 

This may/will crash your computer, override GPU memory, or anything possible.

This works on my computer. 

PS. Dont use open source xorg gpu drivers, they have some weird problems with this one ;)



# UI 
Widgets with name "DEL_" should not be deleted.
So dont use gui.deleteAllWidgets()




# Block/Voxel action reference

-(R,G,B) -- "use" -- voxel code


74,74,74 Carbon       2
119,120,115 Lithium   3
186,186,166 Magnesium 4
119,142,125 Sodium    5
209,213,216 Aluminium 6
117,121,139 Silicon   7
186,140,106 Copper    8
142,129,149 Titanium  9
104,102,107 Lead      10

# Notes :
Ideal delta time for 75hz should be 1.0
