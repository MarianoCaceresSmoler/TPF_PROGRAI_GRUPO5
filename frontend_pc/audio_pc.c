
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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

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

static void loadAudioAssets(void); // To load audio assets with saved files

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static int temperaturas_actuales[4];+

static ALLEGRO_SAMPLE *sndShoot = NULL;
static ALLEGRO_SAMPLE *sndExplosion = NULL;
static ALLEGRO_SAMPLE *sndMothership = NULL;
static ALLEGRO_SAMPLE *sndGameOver = NULL;
static ALLEGRO_SAMPLE *bgGameMusic = NULL;
static ALLEGRO_SAMPLE *bgMenuMusic = NULL;
static ALLEGRO_SAMPLE_ID idMenuMusic = {._id = -1};
static ALLEGRO_SAMPLE_ID idGameMusic = {._id = -1};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initAudio(void)
{
    if (!al_install_audio())
    {
        fprintf(stderr, "Error initiating system audio.\n");
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Error updating audio codecs.\n");
    }

    if (!al_reserve_samples(6))
    { // 66 samples (shoot, explosion, music, etc.)
        fprintf(stderr, "Error reserving audio samples.\n");
    }

    loadAudioAssets();
}

void playShootSound(void)
{
    if (sndShoot)
        al_play_sample(sndShoot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void playExplosionSound(void)
{
    if (sndExplosion)
        al_play_sample(sndExplosion, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void playMothershipSound(void)
{
    if (sndMothership)
        al_play_sample(sndMothership, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void playShipDiedSound(void)
{
    if (sndExplosion)
        al_play_sample(sndExplosion, 1.0, 0.0, 0.7, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void playGameoverSound(void)
{
    if (sndGameOver)
        al_play_sample(sndGameOver, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void playMenuMusic(void)
{
    if (bgMenuMusic)
        al_play_sample(bgMenuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &idMenuMusic);
}

void stopMenuMusic(void)
{
    if (idMenuMusic._id != -1)
    {
        al_stop_sample(&idMenuMusic);
    }
}

void playGameplayMusic(void)
{
    if (bgGameMusic)
        al_play_sample(bgGameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &idGameMusic);
}

void stopGameplayMusic(void)
{
    if (idGameMusic._id != -1)
        al_stop_sample(&idGameMusic);
}

void cleanupAudio(void)
{
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
    al_uninstall_audio();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void loadAudioAssets(void)
{
    // Sound effects
    sndShoot = al_load_sample("frontend_pc/assets/audio/shoot.wav");
    if (!sndShoot)
        fprintf(stderr, "Error cargando shoot.wav\n");
    sndExplosion = al_load_sample("frontend_pc/assets/audio/explosion.wav");
    if (!sndExplosion)
        fprintf(stderr, "Error cargando explosion.wav\n");
    sndMothership = al_load_sample("frontend_pc/assets/audio/mothership.wav");
    if (!sndMothership)
        fprintf(stderr, "Error cargando mothership.wav\n");
    sndGameOver = al_load_sample("frontend_pc/assets/audio/gameover.wav");
    if (!sndGameOver)
        fprintf(stderr, "Error cargando gameover.wav\n");

    // Background music
    bgGameMusic = al_load_sample("frontend_pc/assets/audio/gamemusic.wav");
    if (!bgGameMusic)
        fprintf(stderr, "Error cargando gamemusic.wav\n");
    bgMenuMusic = al_load_sample("frontend_pc/assets/audio/menumusic.wav");
    if (!bgMenuMusic)
        fprintf(stderr, "Error cargando menumusic.wav\n");

    // Error managing for samples loaded
    if (!sndShoot || !sndExplosion || !sndMothership || !sndGameOver || !bgGameMusic || !bgMenuMusic)
    {
        fprintf(stderr, "Error loading audio files\n");
        cleanupAudio();
    }
}