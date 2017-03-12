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

//Creat window
CDisplayWindow3D	win("Raghavender Sahdev MRPT / OpenGL c++ demo",1080,720);
COpenGLScenePtr &theScene = win.get3DSceneAndLock();


// define parameters for cube pose and define the cube itself here
double trans_x=0, trans_y=0, trans_z=0;
double yaw=0, pitch=0, roll=0;
opengl::CBoxPtr cube_obj = opengl::CBox::Create(TPoint3D(0,0,0),TPoint3D(2,2,2), true, 5.0);
opengl::CTextPtr cube_label = opengl::CText::Create("Hollow Cube");
mrpt::poses::CPose3D cube_pose(trans_x, trans_y, trans_z, yaw, pitch, roll);

// define the pose of the Robot in 2d plane and define the robot and the laser / camera here
double x_robot = 6 , y_robot = 0, yaw_robot = 0, update_dist = 0.2, update_yaw = 0.2;
opengl::CSetOfObjectsPtr pioneer = opengl::stock_objects::RobotPioneer();
mrpt::poses::CPose3D robot_pose(x_robot, y_robot, 0, yaw_robot, 0, 0); // 2D plane movement
mrpt::poses::CPose3D camera_pose(x_robot, y_robot, 0, yaw_robot, 0, 0); // 2D plane movement

//create laser / camera and robot label here
opengl::CSetOfObjectsPtr laser = opengl::stock_objects::Hokuyo_URG();
opengl::CTextPtr pioneer_label = opengl::CText::Create("Pioneer 3AT");


class MyObserver : public mrpt::utils::CObserver
{
protected:
    void OnEvent(const mrptEvent &e)
    {
       if (e.isOfType<mrptEventWindowChar>())
        {
            const mrptEventWindowChar &ee = static_cast<const mrptEventWindowChar &>(e);
            //cout << "character entered " << ee.source_object<< ". Char code: " <<  ee.char_code << endl;
            key_code = ee.char_code;

            cube_obj->setLocation(0,0,0);
            cube_obj->setColor(1,1,0);



            //a = 97, d=100, s=115, w=119, z=122, x=120
            //0=48, 1=49,2=50,3=51,4=52,5=53,6=54,7=55,8=56,9=57
            double update1 = 0.5, update2 = 0.2;
            if(key_code == 97 || key_code == 65)
                trans_x = trans_x + update1;
            if(key_code == 100 || key_code == 68)
                trans_x = trans_x - update1;
            if(key_code == 119 || key_code == 87)
                trans_z = trans_z + update1;
            if(key_code == 115 || key_code == 83)
                trans_z = trans_z - update1;
            if(key_code == 122 || key_code == 90)
                trans_y = trans_y + update1;
            if(key_code == 120 || key_code == 88)
                trans_y = trans_y - update1;

            if(key_code == 56)
                yaw = yaw + update2;
            if(key_code == 50)
                yaw = yaw - update2;
            if(key_code == 52)
                pitch = pitch + update2;
            if(key_code == 54)
                pitch = pitch - update2;
            if(key_code == 49)
                yaw = yaw + update2;
            if(key_code == 48)
                yaw = yaw - update2;




            cube_pose.setFromValues(trans_x, trans_y, trans_z, yaw, pitch, roll);
            cube_obj->setPose(cube_pose);
            theScene->insert( cube_obj );

            cube_label->setPose(cube_pose);//setLocation(cubes_x,0,0);
            theScene->insert(cube_label);

            //cout << yaw << endl << cube_obj->getPose() << endl;

            //up= 315, down=317, right=316, left=314
            if(key_code == 315)
            {
                x_robot = x_robot + update_dist * cos(yaw_robot);
                y_robot = y_robot + update_dist * sin(yaw_robot);
            }
            if(key_code == 317)
            {
                x_robot = x_robot - update_dist * cos(yaw_robot);
                y_robot = y_robot - update_dist * sin(yaw_robot);
            }
            if(key_code == 316)
                yaw_robot -= update_yaw;
            if(key_code == 314)
                yaw_robot += update_yaw;

           robot_pose.setFromValues(x_robot, y_robot, 0, yaw_robot, 0, 0);
           pioneer->setPose(robot_pose);
           pioneer->setScale(4,4,4);
           theScene->insert( pioneer );
           //insert the label with the robot
           pioneer_label->setPose(robot_pose);
           theScene->insert(pioneer_label);

           camera_pose.setFromValues(x_robot,y_robot, 1, yaw_robot+1.6, 0, 1.6);
           laser->setPose(camera_pose);
           laser->setScale(4,4,4);
           theScene->insert( laser );

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

    cout << "\n\nDemo C++ application by Raghavender Sahdev\n" << endl
            << "Instructions for moving robot and cube\n" << endl
            << "CUBE: \n\n a/d translate along x axis \n z-x translate along y-axis \n w-s translate along z axis \n" << endl
            << "number keys: \n 8-2: increase-decrease yaw \n 4-6: increase decrease pitch \n 1-0 increase-decrease roll \n\n" << endl
            << "ROBOT: \n\n up-down arrows for moving front and back \n left-right arrows for turning the robot"<<endl
            << "ENJOY THE App" << endl;

    // XY Grid
    int x_max =14,y_max=14, z_max=14;
    opengl::CGridPlaneXYPtr xy_grid = opengl::CGridPlaneXY::Create(-x_max,x_max,-y_max,y_max,0,1);
    xy_grid->setColor(0.7,0.7,0.7);
    xy_grid->setLocation(0,0,0);
    xy_grid->setScale(1,1,1);
    theScene->insert( xy_grid );

    //XY grid label
    opengl::CTextPtr xy_grid_label = opengl::CText::Create("X-Y Plane");
    xy_grid_label->setLocation(0,20,0);
    theScene->insert(xy_grid_label);


    //create the 3d Axis here
    opengl::CAxisPtr axis_xyz = opengl::CAxis::Create(-x_max,-y_max,-z_max, x_max,y_max,z_max, 2,2, true);
    axis_xyz->setLocation(0,0,0);
    theScene->insert( axis_xyz );

    win.setCameraZoom(100);
    win.unlockAccess3DScene();
    win.repaint();
    cout << "Close the window to end.\n";

    while (win.isOpen())
    {
        //win.addTextMessage(5,5, format("%.01fFPS", win.getRenderingFPS()));
        mrpt::system::sleep(2);
        win.repaint();
    }
    return 0;
}

