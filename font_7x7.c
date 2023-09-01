#include "surface.h"
#include "font.h"

//font is 7x7, from ascii code 32 to 126 (inclusive)


const char font_7x7[] = "" \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    \
    "#      " \
    "#      " \
    "#      " \
    "#      " \
    "#      " \
    "       " \
    "#      " \
    \
    " ## ## " \
    "  #  # " \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    \
    "       " \
    "  # #  " \
    " ##### " \
    "  # #  " \
    " ##### " \
    "  # #  " \
    "       " \
    \
    "  # #  " \
    " ######" \
    "# # #  " \
    " ##### " \
    "  # # #" \
    "###### " \
    "  # #  " \
    \
    "##    #" \
    "##   # " \
    "    #  " \
    "   #   " \
    "  #    " \
    " #   ##" \
    "#    ##" \
    \
    "       " \
    "   ##  " \
    "  #  # " \
    " ##### " \
    "# #  # " \
    "#  ### " \
    " ### ##" \
    \
    "  ##   " \
    "   #   " \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    \
    "      #" \
    "     # " \
    "    #  " \
    "    #  " \
    "    #  " \
    "     # " \
    "      #" \
    \
    "#      " \
    " #     " \
    "  #    " \
    "  #    " \
    "  #    " \
    " #     " \
    "#      " \
    \
    " # # # " \
    "  ###  " \
    " # # # " \
    "       " \
    "       " \
    "       " \
    "       " \
    \
    "       " \
    "   #   " \
    "   #   " \
    " ##### " \
    "   #   " \
    "   #   " \
    "       " \
    \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    "##     " \
    " #     " \
    \
    "       " \
    "       " \
    "       " \
    " ##### " \
    "       " \
    "       " \
    "       " \
    \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    "##     " \
    "##     " \
    \
    "      #" \
    "     # " \
    "    #  " \
    "   #   " \
    "  #    " \
    " #     " \
    "#      " \
    \
    " ##### " \
    "#     #" \
    "#  #  #" \
    "#  #  #" \
    "#  #  #" \
    "#     #" \
    " ##### " \
    \
    "   #   " \
    "  ##   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "  ###  " \
    \
    " ##### " \
    "#     #" \
    "      #" \
    "   ### " \
    " ##    " \
    "#      " \
    "#######" \
    \
    " ##### " \
    "#     #" \
    "      #" \
    "    ## " \
    "      #" \
    "#     #" \
    " ##### " \
    \
    "#      " \
    "#      " \
    "#   #  " \
    "#   #  " \
    " ######" \
    "    #  " \
    "    #  " \
    \
    "#######" \
    "#      " \
    "###### " \
    "      #" \
    "      #" \
    "#     #" \
    " ##### " \
    \
    " ##### " \
    "#     #" \
    "#      " \
    "###### " \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "#######" \
    "      #" \
    "     # " \
    "    #  " \
    "    #  " \
    "    #  " \
    "    #  " \
    \
    " ##### " \
    "#     #" \
    " ##### " \
    "#     #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    " ##### " \
    "#     #" \
    "#     #" \
    " ######" \
    "      #" \
    "#     #" \
    " ##### " \
    \
    "       " \
    "#      " \
    "#      " \
    "       " \
    "#      " \
    "#      " \
    "       " \
    \
    "       " \
    " #     " \
    " #     " \
    "       " \
    " #     " \
    "#      " \
    "       " \
    \
    "      #" \
    "     # " \
    "    #  " \
    "   #   " \
    "    #  " \
    "     # " \
    "      #" \
    \
    "       " \
    "       " \
    " ##### " \
    "       " \
    " ##### " \
    "       " \
    "       " \
    \
    "#      " \
    " #     " \
    "  #    " \
    "   #   " \
    "  #    " \
    " #     " \
    "#      " \
    \
    " ###   " \
    "#   #  " \
    "#   #  " \
    "  ##   " \
    "  #    " \
    "       " \
    "  #    " \
    \
    " ##### " \
    "#     #" \
    "#  ## #" \
    "# #   #" \
    "#  ### " \
    "#      " \
    " ##### " \
    \
    " ##### " \
    "#     #" \
    "#     #" \
    "#######" \
    "#     #" \
    "#     #" \
    "#     #" \
    \
    "###### " \
    "#     #" \
    "#     #" \
    "###### " \
    "#     #" \
    "#     #" \
    "###### " \
    \
    " ##### " \
    "#     #" \
    "#      " \
    "#      " \
    "#      " \
    "#     #" \
    " ##### " \
    \
    "#####  " \
    "#    # " \
    "#     #" \
    "#     #" \
    "#     #" \
    "#    # " \
    "#####  " \
    \
    "#######" \
    "#      " \
    "#      " \
    "#####  " \
    "#      " \
    "#      " \
    "#######" \
    \
    "#######" \
    "#      " \
    "#      " \
    "#####  " \
    "#      " \
    "#      " \
    "#      " \
    \
    " ##### " \
    "#     #" \
    "#      " \
    "#      " \
    "#   ###" \
    "#     #" \
    " ##### " \
    \
    "#     #" \
    "#     #" \
    "#     #" \
    "#######" \
    "#     #" \
    "#     #" \
    "#     #" \
    \
    "#######" \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "#######" \
    \
    "     ##" \
    "      #" \
    "      #" \
    "      #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "#     #" \
    "#    # " \
    "#  ##  " \
    "###    " \
    "#  ##  " \
    "#    # " \
    "#     #" \
    \
    "#      " \
    "#      " \
    "#      " \
    "#      " \
    "#      " \
    "#      " \
    "#######" \
    \
    "#     #" \
    "##   ##" \
    "# # # #" \
    "#  #  #" \
    "#     #" \
    "#     #" \
    "#     #" \
    \
    "#     #" \
    "##    #" \
    "# #   #" \
    "#  #  #" \
    "#   # #" \
    "#    ##" \
    "#     #" \
    \
    " ##### " \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "###### " \
    "#     #" \
    "#     #" \
    "###### " \
    "#      " \
    "#      " \
    "#      " \
    \
    " ##### " \
    "#     #" \
    "#     #" \
    "#     #" \
    "#   # #" \
    "#    # " \
    " #### #" \
    \
    "###### " \
    "#     #" \
    "#     #" \
    "###### " \
    "#   #  " \
    "#    # " \
    "#     #" \
    \
    " ######" \
    "#      " \
    "#      " \
    " ##### " \
    "      #" \
    "      #" \
    "###### " \
    \
    "#######" \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    " #   # " \
    "  # #  " \
    "   #   " \
    \
    "#     #" \
    "#     #" \
    "#     #" \
    "#  #  #" \
    "#  #  #" \
    "#  #  #" \
    " ## ## " \
    \
    "#     #" \
    "#     #" \
    " #   # " \
    "  ###  " \
    " #   # " \
    "#     #" \
    "#     #" \
    \
    "#     #" \
    "#     #" \
    "#     #" \
    " ######" \
    "      #" \
    "#     #" \
    " ##### " \
    \
    "#######" \
    "     # " \
    "    #  " \
    "   #   " \
    "  #    " \
    " #     " \
    "#######" \
    \
    "  ###  " \
    "  #    " \
    "  #    " \
    "  #    " \
    "  #    " \
    "  #    " \
    "  ###  " \
    \
    "#      " \
    " #     " \
    "  #    " \
    "   #   " \
    "    #  " \
    "     # " \
    "      #" \
    \
    "  ###  " \
    "    #  " \
    "    #  " \
    "    #  " \
    "    #  " \
    "    #  " \
    "  ###  " \
    \
    "   #   " \
    "  # #  " \
    " #   # " \
    "       " \
    "       " \
    "       " \
    "       " \
    \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    "#######" \
    \
    "  ##   " \
    "   ##  " \
    "       " \
    "       " \
    "       " \
    "       " \
    "       " \
    \
    "       " \
    " ##### " \
    "      #" \
    " ######" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "#      " \
    "#      " \
    "###### " \
    "#     #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "       " \
    "       " \
    " ##### " \
    "#     #" \
    "#      " \
    "#     #" \
    " ##### " \
    \
    "      #" \
    "      #" \
    " ######" \
    "#     #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "       " \
    "       " \
    " ##### " \
    "#     #" \
    "###### " \
    "#      " \
    " ######" \
    \
    "    ## " \
    "   #  #" \
    "   #   " \
    "  ###  " \
    "   #   " \
    "   #   " \
    "###    " \
    \
    "       " \
    " ##### " \
    "#     #" \
    " ##### " \
    "      #" \
    "#     #" \
    " ##### " \
    \
    "#      " \
    "#      " \
    "###### " \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    \
    "       " \
    "   ##  " \
    "       " \
    "  #### " \
    "   #   " \
    "   #   " \
    " ####  " \
    \
    "       " \
    "     ##" \
    "       " \
    "    ###" \
    "      #" \
    "#     #" \
    " ##### " \
    \
    "#      " \
    "#   ## " \
    "#  #   " \
    "###    " \
    "#  ### " \
    "#     #" \
    "#     #" \
    \
    " ##    " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "    ## " \
    \
    "       " \
    "       " \
    " ## ## " \
    "#  #  #" \
    "#  #  #" \
    "#     #" \
    "#     #" \
    \
    "       " \
    "       " \
    "# #### " \
    "##    #" \
    "#     #" \
    "#     #" \
    "#     #" \
    \
    "       " \
    "       " \
    " ##### " \
    "#     #" \
    "#     #" \
    "#     #" \
    " ##### " \
    \
    "       " \
    "       " \
    "###### " \
    "#     #" \
    "###### " \
    "#      " \
    "#      " \
    \
    "       " \
    "       " \
    " ##### " \
    "#    # " \
    " ##### " \
    "     # " \
    "     ##" \
    \
    "       " \
    "       " \
    " ##### " \
    "#     #" \
    "#      " \
    "#      " \
    "#      " \
    \
    "       " \
    "       " \
    " ######" \
    "#      " \
    " ##### " \
    "      #" \
    "###### " \
    \
    "       " \
    "#      " \
    "####   " \
    "#      " \
    "#      " \
    "#     #" \
    " ##### " \
    \
    "       " \
    "       " \
    "#     #" \
    "#     #" \
    "#     #" \
    "#     #" \
    " ######" \
    \
    "       " \
    "       " \
    "#     #" \
    "#     #" \
    " #   # " \
    "  # #  " \
    "   #   " \
    \
    "       " \
    "       " \
    "#     #" \
    "#     #" \
    "#  #  #"
    " # # # " \
    "  # #  " \
    \
    "       " \
    "       " \
    "#     #" \
    " #   # " \
    "  ###  " \
    " #   # " \
    "#     #" \
    \
    "       " \
    "       " \
    "#     #" \
    "#     #" \
    " ######" \
    "      #" \
    " ##### " \
    \
    "       " \
    "       " \
    "#######" \
    "     # " \
    "  ###  " \
    " #     " \
    "#######" \
    \
    "    ## " \
    "   #   " \
    "   #   " \
    "  #    " \
    "   #   " \
    "   #   " \
    "    ## " \
    \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    "   #   " \
    \
    " ##    " \
    "   #   " \
    "   #   " \
    "    #  " \
    "   #   " \
    "   #   " \
    " ##    " \
    \
    "       " \
    " ##   #" \
    "#  ### " \
    "       " \
    "       " \
    "       " \
    "       " \
    "";
