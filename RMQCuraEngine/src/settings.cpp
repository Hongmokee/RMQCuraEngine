#include <cctype>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include "utils/logoutput.h"
#include "utils/string.h"

#include "settings.h"

#define LTRIM_STRING(s) do { while(((s).length() > 0) && isspace((s)[0])) { (s).erase(0, 1); } } while(0)
#define RTRIM_STRING(s) do { while(((s).length() > 0) && isspace((s)[(s).length() - 1])) { (s).erase((s).length() - 1); } } while(0)
#define TRIM_STRING(s) do { LTRIM_STRING(s); RTRIM_STRING(s); } while(0)
#define STRINGIFY(_s) #_s
#define SETTING(name, default) do { _index.push_back(_ConfigSettingIndex(STRINGIFY(name), &name)); name = (default); } while(0)
#define SETTING2(name, altname, default) do { _index.push_back(_ConfigSettingIndex(STRINGIFY(name), &name)); _index.push_back(_ConfigSettingIndex(STRINGIFY(altname), &name)); name = (default); } while(0)

ConfigSettings *ConfigSettings::config = NULL;

ConfigSettings::ConfigSettings()
{
    config = this;
    SETTING2(layerThickness,layer_height, 200);
    SETTING2(initialLayerThickness,bottom_thickness, 300);
    SETTING2(filamentDiameter,filament_diameter, 1750);
    SETTING2(filamentFlow,filament_flow, 100);
    SETTING2(extrusionWidth,nozzle_size, 400);

    SETTING(layer0_width_factor, 100);
    SETTING(layer0extrusionWidth, extrusionWidth*(layer0_width_factor/100));

    SETTING(wall_thickness, 800);
    SETTING(solid_layer_thickness, 800);
    SETTING(solid_bottom, 1);
    SETTING(solid_top, 1);

    SETTING(insetCount, wall_thickness/extrusionWidth);
    SETTING(downSkinCount, solid_layer_thickness/layerThickness);
    SETTING(upSkinCount, solid_layer_thickness/layerThickness);
    SETTING2(skirtDistance,skirt_gap,6000);
    SETTING2(skirtLineCount,skirt_line_count, 1);
    SETTING2(skirtMinLength,skirt_minimal_length, 0);

    SETTING(initialSpeedupLayers, 4);
    SETTING2(initialLayerSpeed,bottom_layer_speed, 20);
    SETTING2(printSpeed,print_speed, 50);
    SETTING2(inset0Speed,inset0_speed, 35);
    SETTING2(insetXSpeed,insetx_speed, printSpeed);
    SETTING2(moveSpeed,travel_speed, 100);

    SETTING(fan_full_height, 500);
    SETTING(fanFullOnLayerNr, fan_full_height/layerThickness);

    SETTING(sparseInfillLineDistance, 100 * extrusionWidth / 20);
    SETTING2(infillOverlap, fill_overlap, 20);
    SETTING2(SupportOverlap, Support_overlap, 15);
    SETTING2(infillSpeed, infill_speed, printSpeed);
    SETTING(infillPattern, INFILL_AUTOMATIC);
    SETTING(skinSpeed, printSpeed);

    SETTING(supportType, SUPPORT_TYPE_GRID);
    SETTING2(supportAngle,support_angle, 60);
    SETTING(supportEverywhere, 0);
    SETTING(supportLineDistance, sparseInfillLineDistance);
    SETTING2(supportXYDistance,support_xy_distance, 700);
    SETTING2(supportZDistance,support_z_distance, 150);
    SETTING(supportExtruder, -1);

    SETTING2(retractionAmount,retraction_amount, 500);
    SETTING(retractionAmountPrime, 0);
    SETTING2(retractionSpeed,retraction_speed, 45);
    SETTING2(retractionAmountExtruderSwitch,retraction_dual_amount, 14500);
    SETTING2(retractionMinimalDistance,retraction_min_travel, 1500);
    SETTING2(minimalExtrusionBeforeRetraction,retraction_minimal_extrusion, 100);
    SETTING2(retractionZHop,retraction_hop, 0);

    SETTING2(enableCombing, retraction_combing, 1);
    SETTING2(enableOozeShield, ooze_shield, 0);

    SETTING(wipe_tower, 0);
    SETTING(wipe_tower_volume, 15);
    SETTING(wipeTowerSize, wipe_tower_volume*wipe_tower);
    SETTING(multiVolumeOverlap, 0);
    SETTING2(objectPosition.X, posx, 80000);
    SETTING2(objectPosition.Y, posy, 85000);
    SETTING2(objectSink,object_sink, 0);
    SETTING(autoCenter, 1);

    SETTING2(raftMargin,raft_margin, 6000);
    SETTING2(raftLineSpacing,raft_line_spacing, 3000);
    SETTING2(raftBaseThickness,raft_base_thickness, 300);
    SETTING2(raftBaseLinewidth,raft_base_linewidth, 1000);
    SETTING2(raftInterfaceThickness,raft_interface_thickness, 270);
    SETTING2(raftInterfaceLinewidth,raft_interface_linewidth, extrusionWidth);
    SETTING(raftInterfaceLineSpacing, 1000);
    SETTING2(raftAirGap,raft_airgap_all, 0);
    SETTING2(raftAirGapLayer0,raft_airgap, layerThickness+20);
    SETTING(raftBaseSpeed, initialLayerSpeed);
    SETTING(raftFanSpeed, 0);
    SETTING2(raftSurfaceThickness,raft_surface_thickness, 270);
    SETTING2(raftSurfaceLinewidth,raft_surface_linewidth, extrusionWidth);
    SETTING(raftSurfaceLineSpacing, extrusionWidth);
    SETTING2(raftSurfaceLayers,raft_surface_layers, 2);
    SETTING(raftSurfaceSpeed, initialLayerSpeed);

    SETTING2(minimalLayerTime,cool_min_layer_time, 5);
    SETTING2(minimalFeedrate,cool_min_feedrate, 10);
    SETTING2(coolHeadLift,cool_head_lift, 0);
    SETTING2(fanSpeedMin,fan_speed, 100);
    SETTING2(fanSpeedMax,fan_speed_max, 100);

    SETTING(fixHorrible, 0);
    SETTING(spiralizeMode, 0);
    SETTING(simpleMode, 0);
    SETTING(gcodeFlavor, GCODE_FLAVOR_REPRAP);

    memset(extruderOffset, 0, sizeof(extruderOffset));
    SETTING(extruderOffset[0].X, 0); // No one says that extruder 0 can not have an offset!
    SETTING(extruderOffset[0].Y, 0);
    SETTING(extruderOffset[1].X, 0);
    SETTING(extruderOffset[1].Y, 0);
    SETTING(extruderOffset[2].X, 0);
    SETTING(extruderOffset[2].Y, 0);
    SETTING(extruderOffset[3].X, 0);
    SETTING(extruderOffset[3].Y, 0);
    SETTING(extruderOffset[4].X, 0);
    SETTING(extruderOffset[4].Y, 0);
    SETTING(extruderOffset[5].X, 0);
    SETTING(extruderOffset[5].Y, 0);
    SETTING(extruderOffset[6].X, 0);
    SETTING(extruderOffset[6].Y, 0);
    SETTING(extruderOffset[7].X, 0);
    SETTING(extruderOffset[7].Y, 0);
    SETTING(extruderOffset[8].X, 0);
    SETTING(extruderOffset[8].Y, 0);
    SETTING(extruderOffset[9].X, 0);
    SETTING(extruderOffset[9].Y, 0);
    SETTING(extruderOffset[10].X, 0);
    SETTING(extruderOffset[10].Y, 0);
    SETTING(extruderOffset[11].X, 0);
    SETTING(extruderOffset[11].Y, 0);
    SETTING(extruderOffset[12].X, 0);
    SETTING(extruderOffset[12].Y, 0);
    SETTING(extruderOffset[13].X, 0);
    SETTING(extruderOffset[13].Y, 0);
    SETTING(extruderOffset[14].X, 0);
    SETTING(extruderOffset[14].Y, 0);
    SETTING(extruderOffset[15].X, 0);
    SETTING(extruderOffset[15].Y, 0);

    startCode =
        "M109 S210     ;Heatup to 210C\n"
        "G21           ;metric values\n"
        "G90           ;absolute positioning\n"
        "G28           ;Home\n"
        "G1 Z15.0 F300 ;move the platform down 15mm\n"
        "G92 E0        ;zero the extruded length\n"
        "G1 F200 E5    ;extrude 5mm of feed stock\n"
        "G92 E0        ;zero the extruded length again\n";
    endCode =
        "M104 S0                     ;extruder heater off\n"
        "M140 S0                     ;heated bed heater off (if you have it)\n"
        "G91                            ;relative positioning\n"
        "G1 E-1 F300                    ;retract the filament a bit before lifting the nozzle, to release some of the pressure\n"
        "G1 Z+0.5 E-5 X-20 Y-20 F9000   ;move Z up a bit and retract filament even more\n"
        "G28 X0 Y0                      ;move X/Y to min endstops, so the head is out of the way\n"
        "M84                         ;steppers off\n"
        "G90                         ;absolute positioning\n";
}

#undef STRINGIFY
#undef SETTING

bool ConfigSettings::setSetting(const char* key, const char* value)
{
    for(unsigned int n=0; n < _index.size(); n++)
    {
        if (stringcasecompare(key, _index[n].key) == 0)
        {
        	//cura::logError("%s\n", value);
        	if(stringcasecompare(value, "True")==0)
        	{
        		*_index[n].ptr = 1;
        		//cura::logError("%d\n",*_index[n].ptr);
        		return true;
        	}
        	else if(stringcasecompare(value, "False")==0)
        	{
        		*_index[n].ptr = 0;
        		//cura::logError("%d\n",*_index[n].ptr);
        		return true;
        	}

        	float tmp = 0;
        	sscanf(value,"%f",&tmp);
        	if(floor(tmp)!=tmp||stringcasecompare(key, "skirt_gap") == 0)
        	{
        		tmp*=1000;
        	}

    		*_index[n].ptr = (int)tmp;
    		//cura::logError("%s\n",_index[n].key);
    		return true;
        }
    }
    if (stringcasecompare(key, "startCode") == 0)
    {
        this->startCode = value;
        return true;
    }
    if (stringcasecompare(key, "endCode") == 0)
    {
        this->endCode = value;
        return true;
    }
    if (stringcasecompare(key, "preSwitchExtruderCode") == 0)
    {
        this->preSwitchExtruderCode = value;
        return true;
    }
    if (stringcasecompare(key, "postSwitchExtruderCode") == 0)
    {
        this->postSwitchExtruderCode = value;
        return true;
    }
    return false;
}

bool ConfigSettings::readSettings(void) {
    return readSettings(DEFAULT_CONFIG_PATH);
}

bool ConfigSettings::readSettings(const char* path) {
    std::ifstream config(path);
    std::string line;
    size_t line_number = 0;

    if(!config.good()) return false;

    while(config.good()) {
        bool multilineContent = false;
        size_t pos = std::string::npos;
        std::getline(config, line);
        line_number += 1;

        // De-comment and trim, skipping anything that shows up empty
        pos = line.find_first_of('#');
        if(pos != std::string::npos) line.erase(pos);
        TRIM_STRING(line);
        if(line.length() == 0) continue;

        // Split into key = val
        std::string key(""), val("");
        pos = line.find_first_of('=');
        if(pos != std::string::npos && line.length() > (pos + 1)) {
            key = line.substr(0, pos);
            val = line.substr(pos + 1);
            TRIM_STRING(key);
            TRIM_STRING(val);
        }

        // Are we about to read a multiline string?
        if(val == CONFIG_MULTILINE_SEPARATOR) {
            val = "";
            multilineContent = true;
            bool done_multiline = false;

            while(config.good() && !done_multiline) {
                std::getline(config, line);
                line_number += 1;

                // We RTRIM the line for two reasons:
                //
                // 1) Make sure that a direct == comparison with '"""' works without
                //    worrying about trailing space.
                // 2) Nobody likes trailing whitespace anyway
                RTRIM_STRING(line);

                // Either accumuliate or terminate
                if(line == CONFIG_MULTILINE_SEPARATOR) {
                    done_multiline = true;
                    // Make sure we don't add an extra trailing newline
                    // to the parsed value
                    RTRIM_STRING(val);
                }
                else {
                    line += "\n";
                    val += line;
                }
            }

            // If we drop out but didn't finish reading, something failed
            if(!done_multiline) {
                cura::logError("Config(%s):L%zd: Failed while reading multiline string.\n", path, line_number);
                return false;
            }

        }

        // Fail if we don't get a key and val
        if(key.length() == 0 || (val.length() == 0 && !multilineContent)) {
            cura::logError("Config(%s): Line %zd: No key value pair found\n", path, line_number);
            return false;
        }

        // Set a config setting for the current K=V
        if(!setSetting(key.c_str(), val.c_str())) {
            cura::logError("Config(%s):L%zd: Failed to set '%s' to '%s'\n", path, line_number, key.c_str(), val.c_str());
            return false;
        }
    }

    return true;
}
