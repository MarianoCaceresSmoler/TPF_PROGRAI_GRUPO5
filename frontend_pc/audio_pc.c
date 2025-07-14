
/***************************************************************************/ /**
    @audio_pc.h
    @Control of music and sound effects
    @Grupo_5
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
// +Incluir el header propio (ej: #include "template.h")+

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

#include "audio_pc.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MUSIC_SAMPLES 6 // number of music samples to reproduce at the same time 

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static void falta_envido (int);+

/**
 * @brief function to load audio assets with saved files
 * @return 0 if the load was successful, -1 otherwise
*/
static int loadAudioAssets(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

// Sounds variables
static ALLEGRO_SAMPLE *sndShoot = NULL;
static ALLEGRO_SAMPLE *sndExplosion = NULL;
static ALLEGRO_SAMPLE *sndMothership = NULL;
static ALLEGRO_SAMPLE_ID idMothershipSound = {._id = -1};
static ALLEGRO_SAMPLE *sndPowerUp = NULL;
static ALLEGRO_SAMPLE *sndGameOver = NULL;

// MUsic variables
static ALLEGRO_SAMPLE *bgGameMusic = NULL;
static ALLEGRO_SAMPLE *bgMenuMusic = NULL;
static ALLEGRO_SAMPLE_ID idMenuMusic = {._id = -1};

// Music instance for gameplay music
static ALLEGRO_SAMPLE_INSTANCE *gameMusicInstance = NULL;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int initAudio(void)
{
    if (!al_install_audio()) // Initialize the audio system
    {
        printf("Error initiating system audio.\n");
        return -1; // return an error if the installation fail
    }

    if (!al_init_acodec_addon()) // Initialize the audio codec
    {
        printf("Error updating audio codecs.\n");
        return -1; // return an error if the audio coded initialization fail
    }

    if (!al_reserve_samples(MUSIC_SAMPLES)) // Reserve samples to reproduce sounds/musics
    {
        printf("Error reserving audio samples.\n");
        return -1; // return an error if the sample reservation fail
    }

    if(loadAudioAssets()) // load private audio files
    {
        printf("Error loading audio assets.\n");
        return -1; // return an error if the audio assets loading fail
    }

    return 0;
}

void playShootSound(void)
{
    if (sndShoot)
        al_play_sample(sndShoot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Play the shoot sound if it exists
}

void playExplosionSound(void)
{
    if (sndExplosion)
        al_play_sample(sndExplosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Play the explosion sound if it exists
}

void playPowerUpSound(void)
{
    if (sndPowerUp)
        al_play_sample(sndPowerUp, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Play the power up sound if it exists
}

void playShipDiedSound(void)
{
    if (sndExplosion)
        al_play_sample(sndExplosion, 1.0, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL); // Play the explosion sound with a lower volume if it exists
}

void playGameoverSound(void)
{
    if (sndGameOver)
        al_play_sample(sndGameOver, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Play the game over sound if it exists
}

void playMenuMusic(void)
{
    if (bgMenuMusic)
        al_play_sample(bgMenuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &idMenuMusic); // Play the menu music in loop if it exists, using its id to stop it later
}

void stopMenuMusic(void)
{
    if (idMenuMusic._id != -1)
    {
        al_stop_sample(&idMenuMusic); // Stop the menu music if it's playing (using its id)
    }
}

unsigned int stopGameplayMusic(void)
{
    unsigned int position = al_get_sample_instance_position(gameMusicInstance);

    al_set_sample_instance_playing(gameMusicInstance, false); // pauses the gameplay music
    return position; // returns the position in which the music was paused
}

void resumeGameplayMusic(unsigned int position)
{
    // resumes gameplay music from the position it was paused
    al_set_sample_instance_position(gameMusicInstance, position);
    al_set_sample_instance_playing(gameMusicInstance, true);
}

void playGameplayMusic(void)
{
    if (gameMusicInstance)
    {
        // Sets the gameplay music sample instance to play from the beginning in loop
        al_set_sample_instance_playmode(gameMusicInstance, ALLEGRO_PLAYMODE_LOOP);
        al_play_sample_instance(gameMusicInstance);
    }
}

void playMothershipSound(void)
{
    if (sndMothership)
        al_play_sample(sndMothership, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &idMothershipSound); // Play the mothership sound in loop if it exists, using its id to stop it later
}

void stopMothershipSound(void)
{
    if (idMothershipSound._id != -1)
    {
        al_stop_sample(&idMothershipSound); // Stop the mothership sound if it's playing (using its id)
    }
}

void cleanupAudio(void)
{
    // Check all sounds and music instances to destroy the ones that were created
    if (sndShoot)
    {
        al_destroy_sample(sndShoot);
        sndShoot = NULL;
    }
    if (sndExplosion)
    {
        al_destroy_sample(sndExplosion);
        sndExplosion = NULL;
    }
    if (sndPowerUp)
    {
        al_destroy_sample(sndPowerUp);
        sndPowerUp = NULL;
    }
    if (sndMothership)
    {
        al_destroy_sample(sndMothership);
        sndMothership = NULL;
    }
    if (sndGameOver)
    {
        al_destroy_sample(sndGameOver);
        sndGameOver = NULL;
    }
    if (bgGameMusic)
    {
        al_destroy_sample(bgGameMusic);
        bgGameMusic = NULL;
    }
    if (bgMenuMusic)
    {
        al_destroy_sample(bgMenuMusic);
        bgMenuMusic = NULL;
    }
    if (gameMusicInstance)
    {
        al_destroy_sample_instance(gameMusicInstance);
        gameMusicInstance = NULL;
    }

    al_uninstall_audio(); // uninstall allegro audio
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int loadAudioAssets(void)
{
    // Load sound effects 
    sndShoot = al_load_sample("frontend_pc/assets/audio/shoot.wav");
    if (!sndShoot)
    {
        printf("Error loading shoot.wav\n");
    }
    sndExplosion = al_load_sample("frontend_pc/assets/audio/explosion.wav");
    if (!sndExplosion)
    {
        printf("Error loading explosion.wav\n");
    }
        
    sndPowerUp = al_load_sample("frontend_pc/assets/audio/powerup.wav");
    if (!sndExplosion)
    {
        printf("Error loading powerup.wav\n");
    }
    sndGameOver = al_load_sample("frontend_pc/assets/audio/gameover.wav");
    if (!sndGameOver)
    {
        printf("Error loading gameover.wav\n");
    }
    sndMothership = al_load_sample("frontend_pc/assets/audio/mothership.wav");
    if (!sndMothership)
    {
        printf("Error loading mothership.wav\n");
    }

    // Load background gameplay music
    bgGameMusic = al_load_sample("frontend_pc/assets/audio/gamemusic.wav");
    if (bgGameMusic)
    {
        gameMusicInstance = al_create_sample_instance(bgGameMusic); // create a sample instance for the gameplay music
        if (!gameMusicInstance)
        {
            printf("Error creating gameplay music sample instance\n");
        }

        if (!al_attach_sample_instance_to_mixer(gameMusicInstance, al_get_default_mixer())) // ataches the sample instance to the default mixer of allegro
        {
            printf("Error connecting gameplay music sample instance to mixer\n");
            al_destroy_sample_instance(gameMusicInstance); // destroy the sample instance created if there was an error with the default mixer
            gameMusicInstance = NULL;
        }
    }
    else
    {
        printf("Error loading gamemusic.wav\n");
    }

    // Load menu music
    bgMenuMusic = al_load_sample("frontend_pc/assets/audio/menumusic.wav");
    if (!bgMenuMusic)
    {
        printf("Error loading menumusic.wav\n");
    }

    // If there was an error loading an audio, cleanup and return -1
    if (!sndShoot || !sndExplosion || !sndMothership || !sndGameOver || !bgGameMusic || !bgMenuMusic)
    {
        printf("Error loading audio files\n");
        cleanupAudio();
        return -1;
    }

    return 0;
}