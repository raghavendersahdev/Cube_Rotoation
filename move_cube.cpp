#include <stdio.h>
#include <stdlib.h>
#include <mrpt/gui/include/mrpt/gui.h>
#include <mrpt/gui/CDisplayWindow3D.h>
#include <mrpt/random.h>
#include <mrpt/system/threads.h>

#include <mrpt/utils/CObserver.h>
#include <mrpt/gui.h>
#include <mrpt/opengl/CGridPlaneXY.h>

#include <mrpt/opengl.h>

using namespace std;
using namespace mrpt;
using namespace mrpt::gui;
using namespace mrpt::opengl;
using namespace mrpt::math;
using namespace mrpt::utils;



CDisplayWindow3D	win("Raghavender Sahdev OpenGL cubes",1080,720);
COpenGLScenePtr &theScene = win.get3DSceneAndLock();

double trans_x=0, trans_y=0, trans_z=0;
double roll=0, pitch=0, yaw=0;
opengl::CBoxPtr cube_obj = opengl::CBox::Create(TPoint3D(0,0,0),TPoint3D(2,2,2), true, 5.0);
opengl::CTextPtr cube_label = opengl::CText::Create("Hollow Cube");


mrpt::poses::CPose3D cube_pose(trans_x, trans_y, trans_z, roll, pitch, yaw);


class MyObserver : public mrpt::utils::CObserver
{
protected:
    void OnEvent(const mrptEvent &e)
    {
       if (e.isOfType<mrptEventWindowChar>())
        {
            const mrptEventWindowChar &ee = static_cast<const mrptEventWindowChar &>(e);
            cout << "[MyObserver] Char event received from: " << ee.source_object<< ". Char code: " <<  ee.char_code << " modif: " << ee.key_modifiers << "\n";
            key_code = ee.char_code;

            cube_obj->setLocation(0,0,0);
            cube_obj->setColor(1,0,0);

            //a = 97, d=100, s=115, w=119, z=122, x=120
            //0=48, 1=49,2=50,3=51,4=52,5=53,6=54,7=55,8=56,9=57
            double update1 = 0.5, update2 = 0.2;
            if(key_code == 97)
                trans_x = trans_x + update1;
            if(key_code == 100)
                trans_x = trans_x - update1;
            if(key_code == 119)
                trans_z = trans_z + update1;
            if(key_code == 115)
                trans_z = trans_z - update1;
            if(key_code == 122)
                trans_y = trans_y + update1;
            if(key_code == 120)
                trans_y = trans_y - update1;

            if(key_code == 56)
                roll = roll + update2;
            if(key_code == 50)
                roll = roll - update2;
            if(key_code == 52)
                pitch = pitch + update2;
            if(key_code == 54)
                pitch = pitch - update2;
            if(key_code == 49)
                yaw = yaw + update2;
            if(key_code == 48)
                yaw = yaw - update2;



            cube_pose.setFromValues(trans_x, trans_y, trans_z, roll, pitch, yaw);
            cube_obj->setPose(cube_pose);
            theScene->insert( cube_obj );

            cube_label->setPose(cube_pose);//setLocation(cubes_x,0,0);
            theScene->insert(cube_label);
        }        
    }
public:
    int key_code;
};
// Observe windows for events.
MyObserver  observer;

int main()
{

    mrpt::global_settings::OCTREE_RENDER_MAX_POINTS_PER_NODE = 10000;

    observer.observeBegin(win);
    int n_cubes;
    cout << "enter number of cubes: ";
    cin >> n_cubes;





    // XY Grid
    int x_max =14,y_max=14, z_max=14;
    opengl::CGridPlaneXYPtr xy_grid = opengl::CGridPlaneXY::Create(-x_max,x_max,-y_max,y_max,0,1);
    xy_grid->setColor(0.7,0.7,0.7);
    xy_grid->setLocation(0,0,0);
    xy_grid->setScale(1,1,1);
    theScene->insert( xy_grid );

    opengl::CTextPtr gl_txt = opengl::CText::Create("CGridPlaneXY");
    gl_txt->setLocation(0,20,0);
    theScene->insert(gl_txt);



    opengl::CAxisPtr axis_xyz = opengl::CAxis::Create(-x_max,-y_max,-z_max, x_max,y_max,z_max, 2,2, true);
    axis_xyz->setLocation(0,0,0);
    theScene->insert( axis_xyz );


    int cubes_x=0;//, cube2_x=4;
    // Box
    {
        opengl::CBoxPtr obj2 = opengl::CBox::Create(TPoint3D(0,0,0),TPoint3D(2,2,2), false);
        obj2->setLocation(cubes_x,6,0);
        obj2->setLineWidth(4);
        obj2->setColor(1,0,0);
        obj2->enableBoxBorder(true);
        theScene->insert( obj2 );


        opengl::CTextPtr label_cube2 = opengl::CText::Create("Solid Cube");
        label_cube2->setColorR(1);
        label_cube2->setLocation(cubes_x,6,0);
        theScene->insert(label_cube2);
    }

    win.setCameraZoom(100);
    win.unlockAccess3DScene();
    win.repaint();
    cout << "Close the window to end.\n";

    while (win.isOpen())
    {
        win.addTextMessage(5,5, format("%.01fFPS", win.getRenderingFPS()));
        mrpt::system::sleep(2);
        win.repaint();
    }
    return 0;
}

