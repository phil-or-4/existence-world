# existence-world
This is another project I did to test my abilities with building data structures to suit my needs.
The goal was to have a 20x20 Grid. Each block of that grid is called a sector. In each sector and can have as many people
as I want. People each have id's, and every person as a unique id. If a person dies, their id can be used by and next 
new born person. Each person can do three thing, move, be born, or die (hence the name "existence-world" since each
person just exists they don't do anything else). 

So the main thing is getting everything to work, the next goal is to make all the operations runtime constant O(1). The functions are declared in the GWInterface.h .

It's pretty simple what I did. I used a Person Array that's indexed by id so I could do lookups in constant time. Now
each sector of the grid has a linked list of Persons to represent which people are in that sector. Each PersonArrObj has
a pointer pointing to their respective PersonNodeObj in a sector list. When I move a person I simply look them up 
in the Person array, go to the PersonNodeObj they are pointing to, yank that node from the sector list and push that node
back in the sector I want to move the person to move to. All that for constant runtime on most of the operations.
