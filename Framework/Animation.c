#ifdef INTRO
/*
char Animation_tab[11][8][15] = 
{
                  {
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,3,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,128,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,15,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,224,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,63,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,248,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,0,248,255,248,0,0,0,0,0,0},
                  {0,0,0,0,0,0,255,191,255,0,0,0,0,0,0},
                  {0,0,0,0,0,0,255,199,255,0,0,0,0,0,0},
                  {0,0,0,0,0,0,255,110,255,0,0,0,0,0,0},
                  {0,0,0,0,0,0,255,219,255,0,0,0,0,0,0},
                  {0,0,0,0,0,0,255,86,255,0,0,0,0,0,0},
                  {0,0,0,0,0,0,255,255,255,0,0,0,0,0,0},
                  {0,0,0,0,0,0,31,255,31,0,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,248,255,255,255,248,0,0,0,0,0},
                  {0,0,0,0,0,255,223,191,127,255,0,0,0,0,0},
                  {0,0,0,0,0,255,123,199,239,255,0,0,0,0,0},
                  {0,0,0,0,0,255,183,110,221,255,0,0,0,0,0},
                  {0,0,0,0,0,255,108,219,182,255,0,0,0,0,0},
                  {0,0,0,0,0,255,155,86,205,255,0,0,0,0,0},
                  {0,0,0,0,0,255,255,255,254,255,0,0,0,0,0},
                  {0,0,0,0,0,31,255,255,255,31,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,254,255,255,255,254,0,0,0,0,0},
                  {0,0,0,0,254,31,223,191,127,255,254,0,0,0,0},
                  {0,0,0,0,255,97,123,199,239,24,255,0,0,0,0},
                  {0,0,0,0,255,216,183,110,221,187,255,0,0,0,0},
                  {0,0,0,0,255,141,108,219,182,121,255,0,0,0,0},
                  {0,0,0,0,255,221,155,86,205,222,255,0,0,0,0},
                  {0,0,0,0,127,255,255,255,254,249,127,0,0,0,0},
                  {0,0,0,0,0,127,255,255,255,127,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,254,255,255,255,254,0,0,0,0,0},
                  {0,0,0,0,224,63,223,191,127,255,224,0,0,0,0},
                  {0,0,0,240,255,97,123,199,239,24,255,240,0,0,0},
                  {0,0,0,255,255,216,183,110,221,187,255,255,0,0,0},
                  {0,0,0,255,255,141,108,219,182,121,255,255,0,0,0},
                  {0,0,0,15,255,221,155,86,205,222,255,15,0,0,0},
                  {0,0,0,0,7,255,255,255,254,249,7,0,0,0,0},
                  {0,0,0,0,0,127,255,255,255,127,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,0,254,255,255,255,254,0,0,0,0,0},
                  {0,0,0,224,63,31,223,191,127,255,63,224,0,0,0},
                  {0,0,240,31,224,97,123,199,239,24,224,31,240,0,0},
                  {0,0,255,0,255,216,183,110,221,187,255,0,255,0,0},
                  {0,0,255,0,255,141,108,219,182,121,255,0,255,0,0},
                  {0,0,15,248,7,221,155,86,205,222,7,248,15,0,0},
                  {0,0,0,7,252,255,255,255,254,249,252,7,0,0,0},
                  {0,0,0,0,0,127,255,255,255,127,0,0,0,0,0},
                  },
                  {
                  {0,0,0,0,248,254,255,255,255,254,248,0,0,0,0},
                  {0,0,224,63,255,31,223,191,127,255,255,63,224,0,0},
                  {0,240,31,224,255,97,123,199,239,24,255,224,31,240,0},
                  {0,255,0,255,255,216,183,110,221,187,255,255,0,255,0},
                  {0,255,0,255,255,141,108,219,182,121,255,255,0,255,0},
                  {0,15,248,7,255,221,155,86,205,222,255,7,248,15,0},
                  {0,0,7,252,255,255,255,255,254,249,255,252,7,0,0},
                  {0,0,0,0,31,127,255,255,255,127,31,0,0,0,0},
                  },

               
                  {
                  {0,0,0,224,56,206,255,255,255,206,56,224,0,0,0},
                  {0,224,63,193,254,31,223,191,127,255,254,193,63,224,0},
                  {240,31,224,255,255,97,123,199,239,24,255,255,224,31,240},
                  {255,0,255,255,255,216,183,110,221,187,255,255,255,0,255},
                  {255,0,255,255,255,141,108,219,182,121,255,255,255,0,255},
                  {15,248,7,255,255,221,155,86,205,222,255,255,7,248,15},
                  {0,7,252,131,127,255,255,255,254,249,127,131,252,7,0},
                  {0,0,0,7,28,115,255,255,255,115,28,7,0,0,0},
                  }
                  
         */    
};
#endif
