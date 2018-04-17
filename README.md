# xt-hd-imager
Project for a hard drive imaging software for the IBM-XT and compatibles (mainly the Brazillian Nexus 2600)

Compiling and running this code on MAME returns a few errors while trying to read Sector 1, Head 1, Track 1, sometimes gives error 64, other times error 128 or 160...

Currently the code onde reads sector 1, head 1, track 1 and returns the biosdisk error code following by the block of 512 chars representing the bytes on the sector, (Z's are the default value, made it so I can clearly see when it starts reading "correctly")
