#ifndef APP_TERRAIN_H
#define APP_TERRAIN_H

#include "evo.h"

/*******************************************************************************
 Climate
/******************************************************************************/
typedef enum {
  TERRAIN_WEATHER_CLOUDY,
  TERRAIN_WEATHER_SUNNY,
  TERRAIN_WEATHER_RAINY
} terrain_WeatherType;
typedef struct {
  terrain_WeatherType weather; /**< the type of weather */
  tvfloat intensity;   /**< the power of the weather type. */
  tvfloat temperature; /**< the temperature at this sample */
  tv_vector2 wind_dir; /**< the directio of the wind */
  tvfloat wind_speed;  /**< the speed of the wind */
}ClimateSample;
COMPONENT(terrain_Climate, tv_component)
  tv_array* /*climateSample*/ typeSamples;
  tvfloat predictability;   /**< how randomly weather moves between samples */
ENDCOMPONENT(terrain_Climate)
void METHOD(terrain_Climate, generate, tvuint numSamples);
void METHOD(terrain_Climate, updateSample, tvuint numSamples);
void METHOD(terrain_Climate, updateSamples, tvuint numSamples);
/*******************************************************************************
 Soil
*******************************************************************************/
typedef enum {
  TERRAIN_SOILTYPE_RICH,
  TERRAIN_SOILTYPE_SAND,
  TERRAIN_SOILTYPE_ROCKY
}terrain_SoilType;
typedef struct {
  terrain_SoilType type;
  tvfloat granularity;
  tvfloat dryness;
}SoilSample;
COMPONENT(terrain_Soil, tv_component)
  tv_array* /*SoilSample*/ samples;
  tvfloat consistency;  /**< the frequency of outliers */
ENDCOMPONENT(terrain_Soil)
void METHOD(terrain_Soil, generate, tvuint numSamples);
void METHOD(terrain_Soil, updateSample, tvuint numSamples);
void METHOD(terrain_Soil, updateSamples, tvuint numSamples);
/*******************************************************************************
 Flora
*******************************************************************************/
typedef enum {
  TERRAIN_FLORATYPE_GRASS,
  TERRAIN_FLORATYPE_BUSH,
  TERRAIN_FLORATYPE_FLOWER,
  TERRAIN_FLORATYPE_TREE
}terrain_FloraType;

typedef struct {
  terrain_FloraType type; /**< the type of the flora sample */
  tvfloat health;         /**< the healthiness of the flora sample */
}terrain_FloraSample;

COMPONENT(terrain_Flora, tv_component)
  tv_array* samples;
ENDCOMPONENT(terrain_Flora)
void METHOD(terrain_Flora, generate, tvuint numSamples);
void METHOD(terrain_Flora, updateSample, tvuint numSamples);
void METHOD(terrain_Flora, updateSamples, tvuint numSamples);
/*******************************************************************************
 Features
*******************************************************************************/
typedef enum {
  TERRAIN_FEATURETYPE_OVERLOOK,
  TERRAIN_FEATURETYPE_MONUMENT,
  TERRAIN_FEATURETYPE_LAVA
}terrain_FeatureType;
typedef struct {
  terrain_FeatureType type;
  tvfloat grandeur;   /**< how exaggerated this feature is. */
}terrain_FeatureSample;

/*******************************************************************************
 Ground (visualizer)
 Ground is the "walkway" of the area.  This is the only part of the terrain that
 the user actually has any interaction with. Because of this it is important 
 that the mesh reflect what actually happens when the player touches it.
 Guidelines:
	Generated terrain should be FLAT.
	Ground may be highly detailed (e.g. blades of grass)
		(of course pay consideration to performance in such cases)
	It may be a good idea to create this detail via tesselation and keep the actual
	mesh a generic strip of quads.  
*******************************************************************************/
COMPONENT(terrain_Ground, tv_component)
  tv_model* mesh;			/**< a mesh generated based on the ground properties. */
  tv_material* material;	/**< the material used to render the sky. */
  terrain_Soil* soil;
ENDCOMPONENT(terrain_Ground)
void METHOD(terrain_Ground, set_soil, terrain_Ground* new_soil);
/*******************************************************************************
 Sky (visualizer) 
*******************************************************************************/
COMPONENT(terrain_Sky, tv_component)
  tv_model* mesh;  /**< a mesh generated based on the ground properties. */
  tv_material* material;  /**< the material used to render the ground. */
ENDCOMPONENT(terrain_Sky)
void METHOD(terrain_Ground, generate, tvuint numSamples, tvfloat sampleUnit);

#endif
