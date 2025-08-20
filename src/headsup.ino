//
//  This sketch uses an M5Stack Core2 (or CoreS3) connected to a Glass2
//  external screen to display some graphics and text.
//
//  License:  MIT. See the LICENSE file in the project root for
//            more details.
//
#include <M5UnitGLASS2.h>
#include <M5Unified.h>
#include <M5GFX.h>

//  Gives us std::vector
#include <vector>


//  Ratio for shape size relative to the screen
const float shapeRatio  = 0.80F;

//  Delay for displaying shapes in loop()
const int   loopDelay   = 1000;

//  Delay between rotations
const int   rotateDelay = 300;

//  Index of the Glass2 screen
int glass2Index;

//  The main screen sprite which reduces screen flicker
//  when updating
M5Canvas *mainScreen;

//  Structure to hold vertices 
typedef struct vertex
{
  int x;
  int y;
} vertex_t;

//  Class to represent a shape
//
//  Note: This class would usually be in a separate header and
//        source file. But, embedding the code makes installation
//        easier
//
class shape
{
  public:
    //  CTOR
    shape (int shapeX, int shapeY, int shapeSize)
    { 
      //  Central X and Y
      centerX   = shapeX; 
      centerY   = shapeY;

      //  Shape size
      size      = shapeSize;
      origSize  = shapeSize;
    };


    //  DTOR    
    ~shape (void)
    {
      vertices.clear(); 
      origVertices.clear();
    }


    //  Drawing and helper functions
    void AddVertex (int x, int y)
    {
      //  Create the vertex
      vertex_t v;
      v.x = x;
      v.y = y;

      //  Track it
      vertices.push_back(v);
    }


    //  Save the original vertices so we can restore
    //  the shape later
    void SaveShape (void)
    {
      origSize      = size;
      origVertices  = vertices;
    }


    //  Restore the shape back to its original settings
    void RestoreShape (void)
    {
      size      = origSize;
      vertices  = origVertices;
    }


    //  Draw the current shape onto a sprite
    void DrawShape (M5Canvas *sprite)
    {
      for (int i = 0; i < vertices.size() - 1; i ++)
      {
        //  Get 2 vertices
        vertex_t base = vertices.at(i);
        vertex_t next = vertices.at(i + 1);

        //  Draw a line
        sprite->drawLine(base.x, base.y, next.x, next.y, WHITE);
      }
    }


    //  Rotate the shape to a given angle
    void RotateShape (int angle)
    {
      //  Convert from degrees to radians which gives
      //  a cleaner rotation
      float adjAngle = float(angle) * (PI / 180);

      //  Loop through all vertices
      for (auto &v : vertices)
      {
        //  Get the current X and Y
        int tmpX = v.x;
        int tmpY = v.y;

        //  Rotate
        v.x = (tmpX - centerX) * cos(adjAngle) - (tmpY - centerY) * sin(adjAngle) + centerX,
        v.y = (tmpX - centerX) * sin(adjAngle) + (tmpY - centerY) * cos(adjAngle) + centerY;
      }
    }

    //  Return the X and Y values
    void getCoords (int &x, int &y)
    {
      x = centerX; 
      y = centerY;
    }


  private:
    //  Center of the shape
    int centerX;
    int centerY;

    //  Size and original size of the shape
    int size;
    int origSize;

    //  Track all vertices
    std::vector<vertex_t> vertices;
    std::vector<vertex_t> origVertices;
};

//  Our shapes
shape *square;
shape *triangle;
shape *pentagon;
shape *hexagon;


//  Common error handler which will output a circle
//  and stop processing
//
//  Note: We get here if we cannot allocate memory for some
//        component, so we can just stop processing
//
void __attribute__((noreturn)) HandleError (void)
{
  //  All displays have at least 20px
  M5.Displays(glass2Index).fillCircle(20, 20, 10, WHITE);
  while (true)
  {
    //  Loop forever
    delay(1000);
  }
}


//  Create the vertices for a shape
void CreateVertices (shape *myShape, const int shapeSize, const int shapeSides)
{
  //  Get the center X and Y coordinates for the shape
  int centerX;
  int centerY;
  myShape->getCoords(centerX, centerY);

  //  Calculate the radius of the shape
  float shapeRadius = shapeSize / 2;

  //  Calculate the angle increment
  float angle       = 0;
  float angleIncr   = (PI * 2.0) / float(shapeSides);

  for (int i = 0; i < shapeSides; i ++)
  {
    //  Add this vertex
    myShape->AddVertex(shapeRadius * cos(angle) + centerX, shapeRadius * sin(angle) + centerY);
    
    //  Next angle
    angle += angleIncr;
  }

  //  Add the last vertex
  myShape->AddVertex(shapeRadius * cos(0) + centerX, shapeRadius * sin(0) + centerY);
}


//  Create a new shape
shape *CreateShape (int centerX, int centerY, int shapeSize, int shapeSides)
{
  //  Create the shape
  shape *myShape = new (std::nothrow) shape(centerX, centerY, shapeSize);
  if (myShape == nullptr)
  {
    //  We did not get the shape
    HandleError();
    //  Processing stops
  }

  //  Create vertices for this shape
  CreateVertices(myShape, shapeSize, shapeSides);

  //  Save the original vertices so the shape can be restored
  myShape->SaveShape();

  //  Return the completed shape
  return myShape;
}


//  Rotate a shape by a given number of degrees
void Rotate (M5Canvas *sprite, shape *myShape, int angle)
{
  //  We also add a progress bar here
  int spriteWidth   = sprite->width();
  int spriteHeight  = sprite->height();

  //  Total number of loops to apply angles
  int totalAngles   = 360 / angle;

  //  Progress width for each iteration
  int progressPx    = ceil(float(spriteWidth) / float(totalAngles - 1));

  //  Rotate the shape
  for (int i = 0; i < totalAngles; i ++)
  {
    //  Rotate and redraw the shape
    myShape->RotateShape(angle);
    myShape->DrawShape(sprite);

    //  Output the current angle
    sprite->setCursor(0, spriteHeight / 2);
    sprite->print("  ");
    sprite->setCursor(0, spriteHeight / 2);
    sprite->printf("%d", i * angle);

    //  Update the progress bar
    sprite->fillRect(0, 0, progressPx * i, 2, WHITE);

    //  Redisplay the screen
    sprite->pushSprite(0, 0);

    //  Wait a bit before we calculate the next angle
    delay(rotateDelay);
  }
}


//  Display and rotate the shape
void DisplayAndRotate (M5Canvas *sprite, shape *myShape, const int angle)
{
  //  Draw the main shape
  myShape->DrawShape(sprite);
  sprite->pushSprite(0, 0);

  //  Rotate the shape 
  Rotate(sprite, myShape, angle);

  //  Restore the shape for the next round
  myShape->RestoreShape();

  //  Short delay
  delay(loopDelay);

  //  Clear the sprite for a new shape
  sprite->fillSprite(BLACK);
}


void setup (void) 
{
  //  We have to configure the Glass2 as an
  //  external screen
  auto cfg = M5.config();
  cfg.external_display.unit_glass2 = true;
  M5.begin(cfg);

  //  Now we can get the screen index for the 
  //  Glass2
  //
  //  Note: If the Glass2 screen is not connected,
  //        output will go to the LCD screen on the
  //        Core2
  //
  glass2Index = M5.getDisplayIndex(m5::board_t::board_M5UnitGLASS2);

  //  Clear the screen and set text size
  M5.Displays(glass2Index).clear();
  M5.Displays(glass2Index).setTextSize(2);

  //  Get screen dimensions
  int screenWidth   = M5.Displays(glass2Index).width();
  int screenHeight  = M5.Displays(glass2Index).height();
  int halfScrWidth  = screenWidth / 2;
  int halfScrHeight = screenHeight / 2;

  //  Create the full screen sprite
  mainScreen = new (std::nothrow) M5Canvas(&M5.Displays(glass2Index));
  if (mainScreen == nullptr)
  {
    //  We did not get the sprite
    HandleError();
    //  Processing stops
  }
  
  //  The sprite which has a 1-bit color depth e.g. pixels are on or off
  mainScreen->setColorDepth(1);
  mainScreen->createSprite(screenWidth, screenHeight);

  //  Determine the size of the shapes which will be a percentage
  //  of the smallest screen dimension
  int shapeSize = min(screenWidth, screenHeight) * shapeRatio;

  //  Create a square
  square = CreateShape(halfScrWidth, halfScrHeight, shapeSize, 4);

  //  Create a triangle
  triangle = CreateShape(halfScrWidth, halfScrHeight, shapeSize, 3);

  //  Create a pentagon
  pentagon = CreateShape(halfScrWidth, halfScrHeight, shapeSize, 5);

  //  Create a hexagon
  hexagon = CreateShape(halfScrWidth, halfScrHeight, shapeSize, 6);
}


void loop (void)
{
  M5.update();

  //  Display and rotate the square
  DisplayAndRotate(mainScreen, square, 60);

  //  Display and rotate the triangle
  DisplayAndRotate(mainScreen, triangle, 90);

  //  Display and rotate the pentagon
  DisplayAndRotate(mainScreen, pentagon, 88);

  //  Display and rotate the hexagon
  DisplayAndRotate(mainScreen, hexagon, 75);
}
