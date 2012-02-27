#include "testApp.h"

//--------------------------------------------------------------
void testApp::parseOsc()
{
    // get the next message
    ofxOscMessage m;
    receiver.getNextMessage( &m );

    // check for quads corner x movements
    if ( m.getAddress() == "/corners/x" )
    {
        // arguments are iif
        int osc_quad = m.getArgAsInt32( 0 );
        int osc_corner = m.getArgAsInt32( 1 );
        float osc_coord = m.getArgAsFloat( 2 );
        quads[osc_quad].corners[osc_corner].x = osc_coord;
    }
    // check for quads corner y movements
    else if ( m.getAddress() == "/corners/y" )
    {
        // arguments are iif
        int osc_quad = m.getArgAsInt32( 0 );
        int osc_corner = m.getArgAsInt32( 1 );
        float osc_coord = m.getArgAsFloat( 2 );
        quads[osc_quad].corners[osc_corner].y = osc_coord;
    }

    // check for active quad corner x movements
    if ( m.getAddress() == "/active/corners/0" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
	float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[0].x = osc_coord_x;
	quads[activeQuad].corners[0].y = osc_coord_y;
    }

    if ( m.getAddress() == "/active/corners/1" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
	float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[1].x = osc_coord_x;
	quads[activeQuad].corners[1].y = osc_coord_y;
    }

    if ( m.getAddress() == "/active/corners/2" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
	float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[2].x = osc_coord_x;
	quads[activeQuad].corners[2].y = osc_coord_y;
    }

    if ( m.getAddress() == "/active/corners/3" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
	float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[3].x = osc_coord_x;
	quads[activeQuad].corners[3].y = osc_coord_y;
    }


    // resync
    else if ( m.getAddress() == "/projection/resync" )
    {
        resync();
    }

    // stop
    else if ( m.getAddress() == "/projection/stop" )
    {
        stopProjection();
    }

    // start
    else if ( m.getAddress() == "/projection/start" )
    {
        startProjection();
    }

    // save
    else if ( m.getAddress() == "/projection/save" )
    {
        setXml();
        XML.saveFile("_lpmt_settings.xml");
        cout<<"saved settings to data/_lpmt_settings.xml"<<endl;
    }

    // load
    else if ( m.getAddress() == "/projection/load" )
    {
        XML.loadFile("_lpmt_settings.xml");
        getXml();
        cout<<"loaded settings from data/_lpmt_settings.xml"<<endl;
        gui.setPage((activeQuad*3)+2);
    }

    // toggle fullscreen
    else if ( m.getAddress() == "/projection/fullscreen" )
    {
        bFullscreen = !bFullscreen;

        if(!bFullscreen)
        {
            ofSetWindowShape(WINDOW_W, WINDOW_H);
            ofSetFullscreen(false);
            // figure out how to put the window in the center:
            int screenW = ofGetScreenWidth();
            int screenH = ofGetScreenHeight();
            ofSetWindowPosition(screenW/2-WINDOW_W/2, screenH/2-WINDOW_H/2);
        }
        else if(bFullscreen == 1)
        {
            ofSetFullscreen(true);
        }
    }

    // toggle gui
    else if ( m.getAddress() == "/gui" )
    {
        gui.toggleDraw();
        bGui = !bGui;
    }

    // toggle setup
    else if ( m.getAddress() == "/setup" )
    {
        if (isSetup)
        {
            isSetup = False;
            for(int i = 0; i < 36; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isSetup = False;
                }
            }
        }
        else
        {
            isSetup = True;
            for(int i = 0; i < 36; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isSetup = True;
                }
            }
        }
    }

    // connects to mpe server
    else if ( m.getAddress() == "/projection/mpe" )
    {
        mpeSetup();
    }

//------------------------------------------------------
// active quad stuff
//------------------------------------------------------

    // change active quad
    else if ( m.getAddress() == "/active/set" )
    {
        // argument is int32
        int osc_activequad = m.getArgAsInt32( 0 );
        if (osc_activequad <= nOfQuads-1)
        {
            quads[activeQuad].isActive = False;
            activeQuad = osc_activequad;
            quads[activeQuad].isActive = True;
            gui.setPage((activeQuad*3)+2);
        }
    }

    // on/off
    else if ( m.getAddress() == "/active/show" )
    {
        // argument is int32
        int osc_quad_isOn = m.getArgAsInt32( 0 );
        if(osc_quad_isOn == 0) {quads[activeQuad].isOn = false;}
        else if(osc_quad_isOn == 1) {quads[activeQuad].isOn = true;}
    }



    // img stuff on active quad
    else if ( m.getAddress() == "/active/img" )
    {
        quads[activeQuad].imgBg = !quads[activeQuad].imgBg;
    }

    // img on/off
    else if ( m.getAddress() == "/active/img/show" )
    {
        // argument is int32
        int osc_quad_imgBg = m.getArgAsInt32( 0 );
        if(osc_quad_imgBg == 0) {quads[activeQuad].imgBg = false;}
        else if(osc_quad_imgBg == 1) {quads[activeQuad].imgBg = true;}
    }

    // img HFlip
    else if ( m.getAddress() == "/active/img/hmirror" )
    {
        // argument is int32
        int osc_quad_imgHFlip = m.getArgAsInt32( 0 );
        if(osc_quad_imgHFlip == 0) {quads[activeQuad].imgHFlip = false;}
        else if(osc_quad_imgHFlip == 1) {quads[activeQuad].imgHFlip = true;}
    }

    // img VFlip
    else if ( m.getAddress() == "/active/img/vmirror" )
    {
        // argument is int32
        int osc_quad_imgVFlip = m.getArgAsInt32( 0 );
        if(osc_quad_imgVFlip == 0) {quads[activeQuad].imgVFlip = false;}
        else if(osc_quad_imgVFlip == 1) {quads[activeQuad].imgVFlip = true;}
    }

    else if ( m.getAddress() == "/active/img/color" )
    {
        // arguments are ffff
        float img_color_r = m.getArgAsFloat( 0 );
        float img_color_g = m.getArgAsFloat( 1 );
        float img_color_b = m.getArgAsFloat( 2 );
        float img_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].imgColorize.r = img_color_r;
        quads[activeQuad].imgColorize.g = img_color_g;
        quads[activeQuad].imgColorize.b = img_color_b;
        quads[activeQuad].imgColorize.a = img_color_a;
    }

    else if ( m.getAddress() == "/active/img/color/1" )
    {
        // arguments are f
        float img_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.r = img_color_r;
    }

    else if ( m.getAddress() == "/active/img/color/2" )
    {
        // arguments are f
        float img_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.g = img_color_g;
    }

    else if ( m.getAddress() == "/active/img/color/3" )
    {
        // arguments are f
        float img_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.b = img_color_b;
    }

    else if ( m.getAddress() == "/active/img/color/4" )
    {
        // arguments are f
        float img_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.a = img_color_a;
    }

    else if ( m.getAddress() == "/active/img/mult/x" )
    {
        // arguments are f
        float img_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].imgMultX = img_mult_x;
    }

    else if ( m.getAddress() == "/active/img/mult/y" )
    {
        // arguments are f
        float img_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].imgMultY = img_mult_y;
    }


    // blendModes on/off
    else if ( m.getAddress() == "/active/blendmodes/show" )
    {
        // argument is int32
        int osc_quad_bBlendModes = m.getArgAsInt32( 0 );
        if(osc_quad_bBlendModes == 0) {quads[activeQuad].bBlendModes = false;}
        else if(osc_quad_bBlendModes == 1) {quads[activeQuad].bBlendModes = true;}
    }

    // blendModes mode
    else if ( m.getAddress() == "/active/blendmodes/mode" )
    {
        // argument is int32
        int osc_quad_blendMode = m.getArgAsInt32( 0 );
        if(osc_quad_blendMode < 4) {quads[activeQuad].blendMode = osc_quad_blendMode;}
    }

    // solid color stuff
    else if ( m.getAddress() == "/active/solid" )
    {
        quads[activeQuad].colorBg = !quads[activeQuad].colorBg;
    }

    // solid on/off
    else if ( m.getAddress() == "/active/solid/show" )
    {
        // argument is int32
        int osc_quad_colorBg = m.getArgAsInt32( 0 );
        if(osc_quad_colorBg == 0) {quads[activeQuad].colorBg = false;}
        else if(osc_quad_colorBg == 1) {quads[activeQuad].colorBg = true;}
    }

    else if ( m.getAddress() == "/active/solid/color" )
    {
        // arguments are ffff
        float solid_color_r = m.getArgAsFloat( 0 );
        float solid_color_g = m.getArgAsFloat( 1 );
        float solid_color_b = m.getArgAsFloat( 2 );
        float solid_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].bgColor.r = solid_color_r;
        quads[activeQuad].bgColor.g = solid_color_g;
        quads[activeQuad].bgColor.b = solid_color_b;
        quads[activeQuad].bgColor.a = solid_color_a;
    }

    else if ( m.getAddress() == "/active/solid/color/1" )
    {
        // arguments are f
        float solid_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.r = solid_color_r;
    }

    else if ( m.getAddress() == "/active/solid/color/2" )
    {
        // arguments are f
        float solid_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.g = solid_color_g;
    }

    else if ( m.getAddress() == "/active/solid/color/3" )
    {
        // arguments are f
        float solid_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.b = solid_color_b;
    }

    else if ( m.getAddress() == "/active/solid/color/4" )
    {
        // arguments are f
        float solid_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.a = solid_color_a;
    }

    else if ( m.getAddress() == "/active/solid/trans" )
    {
        quads[activeQuad].transBg = !quads[activeQuad].transBg;
    }

    // trans on/off
    else if ( m.getAddress() == "/active/solid/trans/show" )
    {
        // argument is int32
        int osc_quad_transBg = m.getArgAsInt32( 0 );
        if(osc_quad_transBg == 0) {quads[activeQuad].transBg = false;}
        else if(osc_quad_transBg == 1) {quads[activeQuad].transBg = true;}
    }

    else if ( m.getAddress() == "/active/solid/trans/color" )
    {
        // arguments are ffff
        float trans_color_r = m.getArgAsFloat( 0 );
        float trans_color_g = m.getArgAsFloat( 1 );
        float trans_color_b = m.getArgAsFloat( 2 );
        float trans_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].secondColor.r = trans_color_r;
        quads[activeQuad].secondColor.g = trans_color_g;
        quads[activeQuad].secondColor.b = trans_color_b;
        quads[activeQuad].secondColor.a = trans_color_a;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/1" )
    {
        // arguments are f
        float trans_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.r = trans_color_r;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/2" )
    {
        // arguments are f
        float trans_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.g = trans_color_g;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/3" )
    {
        // arguments are f
        float trans_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.b = trans_color_b;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/4" )
    {
        // arguments are f
        float trans_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.a = trans_color_a;
    }

    else if ( m.getAddress() == "/active/solid/trans/duration" )
    {
        // arguments are f
        float trans_duration = m.getArgAsFloat( 0 );
        quads[activeQuad].transDuration = trans_duration;
    }


    // mask stuff on active quad
    else if ( m.getAddress() == "/active/mask" )
    {
        quads[activeQuad].bMask = !quads[activeQuad].bMask;
    }

    // mask on/off
    else if ( m.getAddress() == "/active/mask/show" )
    {
        // argument is int32
        int osc_quad_bMask = m.getArgAsInt32( 0 );
        if(osc_quad_bMask == 0) {quads[activeQuad].bMask = false;}
        else if(osc_quad_bMask == 1) {quads[activeQuad].bMask = true;}
    }

    // mask invert
    else if ( m.getAddress() == "/active/mask/invert" )
    {
        // argument is int32
        int osc_quad_maskInvert = m.getArgAsInt32( 0 );
        if(osc_quad_maskInvert == 0) {quads[activeQuad].maskInvert = false;}
        else if(osc_quad_maskInvert == 1) {quads[activeQuad].maskInvert = true;}
    }

    // img stuff on active quad
    else if ( m.getAddress() == "/active/video" )
    {
        quads[activeQuad].videoBg = !quads[activeQuad].videoBg;
    }

    // video on/off
    else if ( m.getAddress() == "/active/video/show" )
    {
        // argument is int32
        int osc_quad_videoBg = m.getArgAsInt32( 0 );
        if(osc_quad_videoBg == 0) {quads[activeQuad].videoBg = false;}
        else if(osc_quad_videoBg == 1) {quads[activeQuad].videoBg = true;}
    }

    else if ( m.getAddress() == "/active/video/color" )
    {
        // arguments are ffff
        float video_color_r = m.getArgAsFloat( 0 );
        float video_color_g = m.getArgAsFloat( 1 );
        float video_color_b = m.getArgAsFloat( 2 );
        float video_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].videoColorize.r = video_color_r;
        quads[activeQuad].videoColorize.g = video_color_g;
        quads[activeQuad].videoColorize.b = video_color_b;
        quads[activeQuad].videoColorize.a = video_color_a;
    }

    else if ( m.getAddress() == "/active/video/color/1" )
    {
        // arguments are f
        float video_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.r = video_color_r;
    }

    else if ( m.getAddress() == "/active/video/color/2" )
    {
        // arguments are f
        float video_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.g = video_color_g;
    }

    else if ( m.getAddress() == "/active/video/color/3" )
    {
        // arguments are f
        float video_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.b = video_color_b;
    }

    else if ( m.getAddress() == "/active/video/color/4" )
    {
        // arguments are f
        float video_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.a = video_color_a;
    }

    else if ( m.getAddress() == "/active/video/mult/x" )
    {
        // arguments are f
        float video_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].videoMultX = video_mult_x;
    }

    else if ( m.getAddress() == "/active/video/mult/y" )
    {
        // arguments are f
        float video_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].videoMultY = video_mult_y;
    }

    else if ( m.getAddress() == "/active/video/speed" )
    {
        // arguments are f
        float video_speed = m.getArgAsFloat( 0 );
        quads[activeQuad].videoSpeed = video_speed;
    }

    else if ( m.getAddress() == "/active/video/volume" )
    {
        // arguments are i
        int video_volume = m.getArgAsInt32( 0 );
        quads[activeQuad].videoVolume = video_volume;
    }


    else
    {
        // unrecognized message: display on the bottom of the screen
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        for ( int i=0; i<m.getNumArgs(); i++ )
        {
            // get the argument type
            msg_string += m.getArgTypeName( i );
            msg_string += ":";
            // display the argument - make sure we get the right type
            if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
                msg_string += ofToString( m.getArgAsInt32( i ) );
            else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
                msg_string += ofToString( m.getArgAsFloat( i ) );
            else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
                msg_string += m.getArgAsString( i );
            else
                msg_string += "unknown";
        }
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
    }
}
