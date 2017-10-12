/* $Id: media.hpp 5645 2017-09-06 03:44:35Z riza $ */
/*
 * Copyright (C) 2013 Teluu Inc. (http://www.teluu.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __PJSUA2_MEDIA_HPP__
#define __PJSUA2_MEDIA_HPP__

/**
 * @file pjsua2/media.hpp
 * @brief PJSUA2 media operations
 */
#include <pjsua-lib/pjsua.h>
#include <pjsua2/types.hpp>

/** PJSUA2 API is inside pj namespace */
namespace pj
{

/**
 * @defgroup PJSUA2_MED Media
 * @ingroup PJSUA2_Ref
 * @{
 */

using std::string;
using std::vector;

/**
 * This structure contains all the information needed to completely describe
 * a media.
 */
struct MediaFormat
{
    /**
     * The format id that specifies the audio sample or video pixel format.
     * Some well known formats ids are declared in pjmedia_format_id
     * enumeration.
     *
     * @see pjmedia_format_id
     */
    pj_uint32_t		id;

    /**
     * The top-most type of the media, as an information.
     */
    pjmedia_type	type;
};

/**
 * This structure describe detail information about an audio media.
 */
struct MediaFormatAudio : public MediaFormat
{
    unsigned	clockRate;	/**< Audio clock rate in samples or Hz. */
    unsigned	channelCount;	/**< Number of channels.		*/
    unsigned	frameTimeUsec;  /**< Frame interval, in microseconds.	*/
    unsigned	bitsPerSample;	/**< Number of bits per sample.		*/
    pj_uint32_t	avgBps;		/**< Average bitrate			*/
    pj_uint32_t	maxBps;		/**< Maximum bitrate			*/

    /**
     * Construct from pjmedia_format.
     */
    void fromPj(const pjmedia_format &format);

    /**
     * Export to pjmedia_format.
     */
    pjmedia_format toPj() const;
};

/**
 * This structure describe detail information about an video media.
 */
struct MediaFormatVideo : public MediaFormat
{
    unsigned		width;	    /**< Video width. 			*/
    unsigned		height;	    /**< Video height.			*/
    int			fpsNum;	    /**< Frames per second numerator.	*/
    int			fpsDenum;   /**< Frames per second denumerator.	*/
    pj_uint32_t		avgBps;	    /**< Average bitrate.		*/
    pj_uint32_t		maxBps;	    /**< Maximum bitrate.		*/

    /**
     * Construct from pjmedia_format.
     */
    void fromPj(const pjmedia_format &format);

    /**
     * Export to pjmedia_format.
     */
    pjmedia_format toPj() const;
};

/** Array of MediaFormat */
typedef std::vector<MediaFormat> MediaFormatVector;

/**
 * This structure descibes information about a particular media port that
 * has been registered into the conference bridge. 
 */
struct ConfPortInfo
{
    /**
     * Conference port number.
     */
    int			portId;

    /**
     * Port name.
     */
    string		name;

    /**
     * Media audio format information
     */
    MediaFormatAudio	format;

    /**
     * Tx level adjustment. Value 1.0 means no adjustment, value 0 means
     * the port is muted, value 2.0 means the level is amplified two times.
     */
    float		txLevelAdj;

    /**
     * Rx level adjustment. Value 1.0 means no adjustment, value 0 means
     * the port is muted, value 2.0 means the level is amplified two times.
     */
    float		rxLevelAdj;

    /**
     * Array of listeners (in other words, ports where this port is
     * transmitting to.
     */
    IntVector		listeners;

public:
    /**
     * Construct from pjsua_conf_port_info.
     */
    void fromPj(const pjsua_conf_port_info &port_info);
};

/**
 * Media port, corresponds to pjmedia_port
 */
typedef void *MediaPort;

/**
 * Media.
 */
class Media
{
public:
    /**
     * Virtual destructor.
     */
    virtual ~Media();

    /**
     * Get type of the media.
     *
     * @return          The media type.
     */
    pjmedia_type getType() const;

protected:
    /**
     * Constructor.
     */
    Media(pjmedia_type med_type);

private:
    /**
     * Media type.
     */
    pjmedia_type        type;
};

/**
 * Audio Media.
 */
class AudioMedia : public Media
{
public:
    /**
    * Get information about the specified conference port.
    */
    ConfPortInfo getPortInfo() const throw(Error);

    /**
     * Get port Id.
     */
    int getPortId() const;

    /**
     * Get information from specific port id.
     */
    static ConfPortInfo getPortInfoFromId(int port_id) throw(Error);

    /**
     * Establish unidirectional media flow to sink. This media port
     * will act as a source, and it may transmit to multiple destinations/sink.
     * And if multiple sources are transmitting to the same sink, the media
     * will be mixed together. Source and sink may refer to the same Media,
     * effectively looping the media.
     *
     * If bidirectional media flow is desired, application needs to call
     * this method twice, with the second one called from the opposite source
     * media.
     *
     * @param sink		The destination Media.
     */
    void startTransmit(const AudioMedia &sink) const throw(Error);

    /**
     *  Stop media flow to destination/sink port.
     *
     * @param sink		The destination media.
     *
     */
    void stopTransmit(const AudioMedia &sink) const throw(Error);

    /**
     * Adjust the signal level to be transmitted from the bridge to this
     * media port by making it louder or quieter.
     *
     * @param level		Signal level adjustment. Value 1.0 means no
     *				level adjustment, while value 0 means to mute
     *				the port.
     */
    void adjustRxLevel(float level) throw(Error);

    /**
     * Adjust the signal level to be received from this media port (to
     * the bridge) by making it louder or quieter.
     *
     * @param level		Signal level adjustment. Value 1.0 means no
     *				level adjustment, while value 0 means to mute
     *				the port.
     */
    void adjustTxLevel(float level) throw(Error);

    /**
     * Get the last received signal level.
     *
     * @return			Signal level in percent.
     */
    unsigned getRxLevel() const throw(Error);

    /**
     * Get the last transmitted signal level.
     *
     * @return			Signal level in percent.
     */
    unsigned getTxLevel() const throw(Error);

    /**
     * Typecast from base class Media. This is useful for application written
     * in language that does not support downcasting such as Python.
     *
     * @param media		The object to be downcasted
     *
     * @return			The object as AudioMedia instance
     */
    static AudioMedia* typecastFromMedia(Media *media);

    /**
     * Virtual Destructor
     */
    virtual ~AudioMedia();

protected:
    /**
     * Conference port Id.
     */
    int			 id;

protected:
    /**
     * Default Constructor.
     */
    AudioMedia();

    /**
     * This method needs to be called by descendants of this class to register
     * the media port created to the conference bridge and Endpoint's
     * media list.
     *
     * param port  the media port to be registered to the conference bridge.
     *
     */
    void registerMediaPort(MediaPort port) throw(Error);

    /**
     * This method needs to be called by descendants of this class to remove
     * the media port from the conference bridge and Endpoint's media list.
     * Descendant should only call this method if it has registered the media
     * with the previous call to registerMediaPort().
     */
    void unregisterMediaPort();

private:
    pj_caching_pool 	 mediaCachingPool;
    pj_pool_t 		*mediaPool;
};

/** Array of Audio Media */
typedef std::vector<AudioMedia*> AudioMediaVector;

/**
 * This structure contains additional info about AudioMediaPlayer.
 */
struct AudioMediaPlayerInfo
{
    /**
     * Format ID of the payload.
     */
    pjmedia_format_id	formatId;

    /**
     * The number of bits per sample of the file payload. For example,
     * the value is 16 for PCM WAV and 8 for Alaw/Ulas WAV files.
     */
    unsigned		payloadBitsPerSample;

    /**
     * The WAV payload size in bytes.
     */
    pj_uint32_t		sizeBytes;

    /**
     * The WAV payload size in samples.
     */
    pj_uint32_t		sizeSamples;
};

/**
 * Audio Media Player.
 */
class AudioMediaPlayer : public AudioMedia
{
public:
    /** 
     * Constructor.
     */
    AudioMediaPlayer();

    /**
     * Create a file player,  and automatically add this 
     * player to the conference bridge.
     *
     * @param file_name	 The filename to be played. Currently only
     *			 WAV files are supported, and the WAV file MUST be
     *			 formatted as 16bit PCM mono/single channel (any
     *			 clock rate is supported).
     * @param options	 Optional option flag. Application may specify
     *			 PJMEDIA_FILE_NO_LOOP to prevent playback loop.
     */
    void createPlayer(const string &file_name,
		      unsigned options=0) throw(Error);

    /**
     * Create a file playlist media port, and automatically add the port
     * to the conference bridge.
     *
     * @param file_names  Array of file names to be added to the play list.
     *			  Note that the files must have the same clock rate,
     *			  number of channels, and number of bits per sample.
     * @param label	  Optional label to be set for the media port.
     * @param options	  Optional option flag. Application may specify
     *			  PJMEDIA_FILE_NO_LOOP to prevent looping.
     */
    void createPlaylist(const StringVector &file_names,
			const string &label="",
			unsigned options=0) throw(Error);

    /**
     * Get additional info about the player. This operation is only valid
     * for player. For playlist, Error will be thrown.
     *
     * @return		the info.
     */
    AudioMediaPlayerInfo getInfo() const throw(Error);

    /**
     * Get current playback position in samples. This operation is not valid
     * for playlist.
     *
     * @return		   Current playback position, in samples.
     */
    pj_uint32_t getPos() const throw(Error);

    /**
     * Set playback position in samples. This operation is not valid for
     * playlist.
     *
     * @param samples	   The desired playback position, in samples.
     */
    void setPos(pj_uint32_t samples) throw(Error);

    /**
     * Typecast from base class AudioMedia. This is useful for application
     * written in language that does not support downcasting such as Python.
     *
     * @param media		The object to be downcasted
     *
     * @return			The object as AudioMediaPlayer instance
     */
    static AudioMediaPlayer* typecastFromAudioMedia(AudioMedia *media);

    /**
     * Destructor.
     */
    virtual ~AudioMediaPlayer();

public:
    /*
     * Callbacks
     */

    /**
     * Register a callback to be called when the file player reading has
     * reached the end of file, or when the file reading has reached the
     * end of file of the last file for a playlist. If the file or playlist
     * is set to play repeatedly, then the callback will be called multiple
     * times.
     *
     * @return			If the callback returns false, the playback
     * 				will stop. Note that if application destroys
     * 				the player in the callback, it must return
     * 				false here.
     */
    virtual bool onEof()
    { return true; }


private:
    /**
     * Player Id.
     */
    int	playerId;

    /**
     *  Low level PJMEDIA callback
     */
    static pj_status_t eof_cb(pjmedia_port *port,
                              void *usr_data);
};

/**
 * Audio Media Recorder.
 */
class AudioMediaRecorder : public AudioMedia
{
public:
    /**
     * Constructor.
     */
    AudioMediaRecorder();

    /**
     * Create a file recorder, and automatically connect this recorder to
     * the conference bridge. The recorder currently supports recording WAV
     * file. The type of the recorder to use is determined by the extension of
     * the file (e.g. ".wav").
     *
     * @param file_name	 Output file name. The function will determine the
     *			 default format to be used based on the file extension.
     *			 Currently ".wav" is supported on all platforms.
     * @param enc_type	 Optionally specify the type of encoder to be used to
     *			 compress the media, if the file can support different
     *			 encodings. This value must be zero for now.
     * @param max_size	 Maximum file size. Specify zero or -1 to remove size
     *			 limitation. This value must be zero or -1 for now.
     * @param options	 Optional options, which can be used to specify the
     * 			 recording file format. Supported options are
     * 			 PJMEDIA_FILE_WRITE_PCM, PJMEDIA_FILE_WRITE_ALAW,
     * 			 and PJMEDIA_FILE_WRITE_ULAW. Default is zero or
     * 			 PJMEDIA_FILE_WRITE_PCM.
     */
    void createRecorder(const string &file_name,
			unsigned enc_type=0,
			pj_ssize_t max_size=0,
			unsigned options=0) throw(Error);

    /**
     * Typecast from base class AudioMedia. This is useful for application
     * written in language that does not support downcasting such as Python.
     *
     * @param media		The object to be downcasted
     *
     * @return			The object as AudioMediaRecorder instance
     */
    static AudioMediaRecorder* typecastFromAudioMedia(AudioMedia *media);

    /**
     * Destructor.
     */
    virtual ~AudioMediaRecorder();

private:
    /**
     * Recorder Id.
     */
    int	recorderId;
};

/**
 * Tone descriptor (abstraction for pjmedia_tone_desc)
 */
class ToneDesc : public pjmedia_tone_desc
{
public:
    ToneDesc()
    {
	pj_bzero(this, sizeof(*this));
    }
    ~ToneDesc() {}
};

/**
 * Array of tone descriptor.
 */
typedef std::vector<ToneDesc> ToneDescVector;

/**
 * Tone digit (abstraction for pjmedia_tone_digit)
 */
class ToneDigit : public pjmedia_tone_digit
{
public:
    ToneDigit()
    {
	pj_bzero(this, sizeof(*this));
    }
    ~ToneDigit() {}
};

/**
 * Array of tone digits.
 */
typedef std::vector<ToneDigit> ToneDigitVector;

/**
 * A digit in tone digit map
 */
struct ToneDigitMapDigit
{
public:
    string	digit;
    int		freq1;
    int		freq2;
};

/**
 * Tone digit map
 */
typedef std::vector<ToneDigitMapDigit> ToneDigitMapVector;

/**
 * Tone generator.
 */
class ToneGenerator : public AudioMedia
{
public:
    /**
     * Constructor.
     */
    ToneGenerator();

    /**
     * Destructor.
     */
    ~ToneGenerator();

    /**
     * Create tone generator.
     */
    void createToneGenerator(unsigned clock_rate = 16000,
			     unsigned channel_count = 1) throw(Error);

    /**
     * Check if the tone generator is still busy producing some tones.
     * @return		    Non-zero if busy.
     */
    bool isBusy() const;

    /**
     * Instruct the tone generator to stop current processing.
     */
    void stop() throw(Error);

    /**
     * Rewind the playback. This will start the playback to the first
     * tone in the playback list.
     */
    void rewind() throw(Error);

    /**
     * Instruct the tone generator to play single or dual frequency tones
     * with the specified duration. The new tones will be appended to
     * currently playing tones, unless stop() is called before calling this
     * function. The playback will begin as soon as the tone generator is
     * connected to other media.
     *
     * @param tones	    Array of tones to be played.
     * @param loop	    Play the tone in a loop.
     */
    void play(const ToneDescVector &tones,
              bool loop=false) throw(Error);

    /**
     * Instruct the tone generator to play multiple MF digits with each of
     * the digits having individual ON/OFF duration. Each of the digit in the
     * digit array must have the corresponding descriptor in the digit map.
     * The new tones will be appended to currently playing tones, unless
     * stop() is called before calling this function. The playback will begin
     * as soon as the tone generator is connected to a sink media.
     *
     * @param digits	    Array of MF digits.
     * @param loop	    Play the tone in a loop.
     */
    void playDigits(const ToneDigitVector &digits,
                    bool loop=false) throw(Error);

    /**
     * Get the digit-map currently used by this tone generator.
     *
     * @return		    The digitmap currently used by the tone generator
     */
    ToneDigitMapVector getDigitMap() const throw(Error);

    /**
     * Set digit map to be used by the tone generator.
     *
     * @param digit_map	    Digitmap to be used by the tone generator.
     */
    void setDigitMap(const ToneDigitMapVector &digit_map) throw(Error);

private:
    pj_pool_t *pool;
    pjmedia_port *tonegen;
    pjmedia_tone_digit_map digitMap;
};


/*************************************************************************
* Sound device management
*/

/**
 * Audio device information structure.
 */
struct AudioDevInfo
{
    /**
     * The device name
     */
    string name;

    /**
     * Maximum number of input channels supported by this device. If the
     * value is zero, the device does not support input operation (i.e.
     * it is a playback only device).
     */
    unsigned inputCount;

    /**
     * Maximum number of output channels supported by this device. If the
     * value is zero, the device does not support output operation (i.e.
     * it is an input only device).
     */
    unsigned outputCount;

    /**
     * Default sampling rate.
     */
    unsigned defaultSamplesPerSec;

    /**
     * The underlying driver name
     */
    string driver;

    /**
     * Device capabilities, as bitmask combination of pjmedia_aud_dev_cap.
     */
    unsigned caps;

    /**
     * Supported audio device routes, as bitmask combination of
     * pjmedia_aud_dev_route. The value may be zero if the device
     * does not support audio routing.
     */
    unsigned routes;

    /**
     * Array of supported extended audio formats
     */
    MediaFormatVector extFmt;

    /**
     * Construct from pjmedia_aud_dev_info.
     */
    void fromPj(const pjmedia_aud_dev_info &dev_info);

    /**
     * Destructor.
     */
    ~AudioDevInfo();
};

/** Array of audio device info */
typedef std::vector<AudioDevInfo*> AudioDevInfoVector;

/**
 * Audio device manager.
 */
class AudDevManager
{
public:
    /**
     * Get currently active capture sound devices. If sound devices has not been
     * created, it is possible that the function returns -1 as device IDs.
     *
     * @return 			Device ID of the capture device.
     */
    int getCaptureDev() const throw(Error);

    /**
     * Get the AudioMedia of the capture audio device.
     *
     * @return			Audio media for the capture device.
     */
    AudioMedia &getCaptureDevMedia() throw(Error);

    /**
     * Get currently active playback sound devices. If sound devices has not
     * been created, it is possible that the function returns -1 as device IDs.
     *
     * @return 			Device ID of the playback device.
     */
    int getPlaybackDev() const throw(Error);

    /**
     * Get the AudioMedia of the speaker/playback audio device.
     *
     * @return			Audio media for the speaker/playback device.
     */
    AudioMedia &getPlaybackDevMedia() throw(Error);

    /**
     * Select or change capture sound device. Application may call this
     * function at any time to replace current sound device. Calling this 
     * method will not change the state of the sound device (opened/closed).
     * Note that this method will override the mode set by setSndDevMode().
     *
     * @param capture_dev   	Device ID of the capture device.
     */
    void setCaptureDev(int capture_dev) const throw(Error);

    /**
     * Select or change playback sound device. Application may call this
     * function at any time to replace current sound device. Calling this 
     * method will not change the state of the sound device (opened/closed).
     * Note that this method will override the mode set by setSndDevMode().
     *
     * @param playback_dev   	Device ID of the playback device.
     */
    void setPlaybackDev(int playback_dev) const throw(Error);

    /**
     * Enum all audio devices installed in the system.
     *
     * @return			The list of audio device info.
     */
    const AudioDevInfoVector &enumDev() throw(Error);

    /**
     * Set pjsua to use null sound device. The null sound device only provides
     * the timing needed by the conference bridge, and will not interract with
     * any hardware.
     *
     */
    void setNullDev() throw(Error);

    /**
     * Disconnect the main conference bridge from any sound devices, and let
     * application connect the bridge to it's own sound device/master port.
     *
     * @return			The port interface of the conference bridge,
     *				so that application can connect this to it's
     *				own sound device or master port.
     */
    MediaPort *setNoDev();

    /**
     * Set sound device mode.
     * 
     * @param mode		The sound device mode, as bitmask combination 
     *				of #pjsua_snd_dev_mode
     *
     */
    void setSndDevMode(unsigned mode) const throw(Error);

    /**
     * Change the echo cancellation settings.
     *
     * The behavior of this function depends on whether the sound device is
     * currently active, and if it is, whether device or software AEC is
     * being used.
     *
     * If the sound device is currently active, and if the device supports AEC,
     * this function will forward the change request to the device and it will
     * be up to the device on whether support the request. If software AEC is
     * being used (the software EC will be used if the device does not support
     * AEC), this function will change the software EC settings. In all cases,
     * the setting will be saved for future opening of the sound device.
     *
     * If the sound device is not currently active, this will only change the
     * default AEC settings and the setting will be applied next time the
     * sound device is opened.
     *
     * @param tail_msec		The tail length, in miliseconds. Set to zero to
     *				disable AEC.
     * @param options		Options to be passed to pjmedia_echo_create().
     *				Normally the value should be zero.
     *
     */
    void setEcOptions(unsigned tail_msec, unsigned options) throw(Error);

    /**
     * Get current echo canceller tail length.
     *
     * @return			The EC tail length in milliseconds,
     *				If AEC is disabled, the value will be zero.
     */
    unsigned getEcTail() const throw(Error);

    /**
     * Check whether the sound device is currently active. The sound device
     * may be inactive if the application has set the auto close feature to
     * non-zero (the sndAutoCloseTime setting in MediaConfig), or
     * if null sound device or no sound device has been configured via the
     * setNoDev() function.
     */
    bool sndIsActive() const;

    /**
     * Refresh the list of sound devices installed in the system. This method
     * will only refresh the list of audio device so all active audio streams
     * will be unaffected. After refreshing the device list, application MUST
     * make sure to update all index references to audio devices before calling
     * any method that accepts audio device index as its parameter.
     *
     */
    void refreshDevs() throw(Error);

    /**
     * Get the number of sound devices installed in the system.
     *
     * @return 			The number of sound devices installed in the
     * 				system.
     *
     */
    unsigned getDevCount() const;

    /**
     * Get device information.
     *
     * @param id		The audio device ID.
     *
     * @return			The device information which will be filled in
     * 				by this method once it returns successfully.
     */
    AudioDevInfo getDevInfo(int id) const throw(Error);

    /**
     * Lookup device index based on the driver and device name.
     *
     * @param drv_name		The driver name.
     * @param dev_name		The device name.
     *
     * @return			The device ID. If the device is not found,
     * 				Error will be thrown.
     */
    int lookupDev(const string &drv_name,
		  const string &dev_name) const throw(Error);

    /**
     * Get string info for the specified capability.
     *
     * @param cap		The capability ID.
     *
     * @return			Capability name.
     */
    string capName(pjmedia_aud_dev_cap cap) const;

    /**
     * This will configure audio format capability (other than PCM) to the
     * sound device being used. If sound device is currently active, the method
     * will forward the setting to the sound device instance to be applied
     * immediately, if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_EXT_FORMAT capability in AudioDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param format		The audio format.
     * @param keep		Specify whether the setting is to be kept for
     * 				future use.
     *
     */
    void
    setExtFormat(const MediaFormatAudio &format, bool keep=true) throw(Error);

    /**
     * Get the audio format capability (other than PCM) of the sound device
     * being used. If sound device is currently active, the method will forward
     * the request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_EXT_FORMAT capability in AudioDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * @return	    		The audio format.
     *
     */
    MediaFormatAudio getExtFormat() const throw(Error);

    /**
     * This will configure audio input latency control or query capability to
     * the sound device being used. If sound device is currently active,
     * the method will forward the setting to the sound device instance to be
     * applied immediately, if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_LATENCY capability in AudioDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param latency_msec	The input latency.
     * @param keep		Specify whether the setting is to be kept
     *				for future use.
     */
    void
    setInputLatency(unsigned latency_msec, bool keep=true) throw(Error);

    /**
     * Get the audio input latency control or query capability of the sound
     * device being used. If sound device is currently active, the method will
     * forward the request to the sound device. If sound device is currently
     * inactive, and if application had previously set the setting and mark the
     * setting as kept, then that setting will be returned. Otherwise, this
     * method will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_LATENCY capability in AudioDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * @return	    		The audio input latency.
     *
     */
    unsigned getInputLatency() const throw(Error);

    /**
     * This will configure audio output latency control or query capability to
     * the sound device being used. If sound device is currently active,
     * the method will forward the setting to the sound device instance to be
     * applied immediately, if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_LATENCY capability in AudioDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param latency_msec    	The output latency.
     * @param keep		Specify whether the setting is to be kept
     * 				for future use.
     *
     */
    void
    setOutputLatency(unsigned latency_msec, bool keep=true) throw(Error);

    /**
     * Get the audio output latency control or query capability of the sound
     * device being used. If sound device is currently active, the method will
     * forward the request to the sound device. If sound device is currently
     * inactive, and if application had previously set the setting and mark the
     * setting as kept, then that setting will be returned. Otherwise, this
     * method will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_LATENCY capability in AudioDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * @return	    		The audio output latency.
     *
     */
    unsigned getOutputLatency() const throw(Error);

    /**
     * This will configure audio input volume level capability to the
     * sound device being used.
     * If sound device is currently active, the method will forward the
     * setting to the sound device instance to be applied immediately,
     * if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_VOLUME_SETTING capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param volume		The input volume level, in percent.
     * @param keep		Specify whether the setting is to be kept for
     * 				future use.
     *
     */
    void setInputVolume(unsigned volume, bool keep=true) throw(Error);

    /**
     * Get the audio input volume level capability of the sound device being
     * used. If sound device is currently active, the method will forward the
     * request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_VOLUME_SETTING capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.     *

     * @return	    		The audio input volume level, in percent.
     *
     */
    unsigned getInputVolume() const throw(Error);

    /**
     * This will configure audio output volume level capability to the sound
     * device being used. If sound device is currently active, the method will
     * forward the setting to the sound device instance to be applied
     * immediately, if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param volume		The output volume level, in percent.
     * @param keep		Specify whether the setting is to be kept
     * 				for future use.
     *
     */
    void setOutputVolume(unsigned volume, bool keep=true) throw(Error);

    /**
     * Get the audio output volume level capability of the sound device being
     * used. If sound device is currently active, the method will forward the
     * request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * @return	    		The audio output volume level, in percent.
     *
     */
    unsigned getOutputVolume() const throw(Error);

    /**
     * Get the audio input signal level capability of the sound device being
     * used. If sound device is currently active, the method will forward the
     * request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_SIGNAL_METER capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * @return	    		The audio input signal level, in percent.
     *
     */
    unsigned getInputSignal() const throw(Error);

    /**
     * Get the audio output signal level capability of the sound device being
     * used. If sound device is currently active, the method will forward the
     * request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_SIGNAL_METER capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * @return	    		The audio output signal level, in percent.
     *
     */
    unsigned getOutputSignal() const throw(Error);

    /**
     * This will configure audio input route capability to the sound device
     * being used. If sound device is currently active, the method will
     * forward the setting to the sound device instance to be applied
     * immediately, if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_ROUTE capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param route		The audio input route.
     * @param keep		Specify whether the setting is to be kept
     * 				for future use.
     *
     */
    void
    setInputRoute(pjmedia_aud_dev_route route, bool keep=true) throw(Error);

    /**
     * Get the audio input route capability of the sound device being used.
     * If sound device is currently active, the method will forward the
     * request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_INPUT_ROUTE capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * @return	    		The audio input route.
     *
     */
    pjmedia_aud_dev_route getInputRoute() const throw(Error);

    /**
     * This will configure audio output route capability to the sound device
     * being used. If sound device is currently active, the method will
     * forward the setting to the sound device instance to be applied
     * immediately, if it supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_ROUTE capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param route		The audio output route.
     * @param keep		Specify whether the setting is to be kept
     * 				for future use.
     *
     */
    void
    setOutputRoute(pjmedia_aud_dev_route route, bool keep=true) throw(Error);

    /**
     * Get the audio output route capability of the sound device being used.
     * If sound device is currently active, the method will forward the
     * request to the sound device. If sound device is currently inactive,
     * and if application had previously set the setting and mark the setting
     * as kept, then that setting will be returned. Otherwise, this method
     * will raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_AUD_DEV_CAP_OUTPUT_ROUTE capability in AudioDevInfo.caps
     * flags, otherwise Error will be thrown.
     *
     * @return	    		The audio output route.
     *
     */
    pjmedia_aud_dev_route getOutputRoute() const throw(Error);

    /**
     * This will configure audio voice activity detection capability to
     * the sound device being used. If sound device is currently active,
     * the method will forward the setting to the sound device instance
     * to be applied immediately, if it supports it.
     *
     * This method is only valid if the device has PJMEDIA_AUD_DEV_CAP_VAD
     * capability in AudioDevInfo.caps flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param enable		Enable/disable voice activity detection
     *				feature. Set true to enable.
     * @param keep		Specify whether the setting is to be kept for
     *				future use.
     *
     */
    void setVad(bool enable, bool keep=true) throw(Error);

    /**
     * Get the audio voice activity detection capability of the sound device
     * being used. If sound device is currently active, the method will
     * forward the request to the sound device. If sound device is currently
     * inactive, and if application had previously set the setting and mark
     * the setting as kept, then that setting will be returned. Otherwise,
     * this method will raise error.
     *
     * This method is only valid if the device has PJMEDIA_AUD_DEV_CAP_VAD
     * capability in AudioDevInfo.caps flags, otherwise Error will be thrown.
     *
     * @return	    		The audio voice activity detection feature.
     *
     */
    bool getVad() const throw(Error);

    /**
     * This will configure audio comfort noise generation capability to
     * the sound device being used. If sound device is currently active,
     * the method will forward the setting to the sound device instance
     * to be applied immediately, if it supports it.
     *
     * This method is only valid if the device has PJMEDIA_AUD_DEV_CAP_CNG
     * capability in AudioDevInfo.caps flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param enable		Enable/disable comfort noise generation
     *				feature. Set true to enable.
     * @param keep		Specify whether the setting is to be kept for
     *				future use.
     *
     */
    void setCng(bool enable, bool keep=true) throw(Error);

    /**
     * Get the audio comfort noise generation capability of the sound device
     * being used. If sound device is currently active, the method will
     * forward the request to the sound device. If sound device is currently
     * inactive, and if application had previously set the setting and mark
     * the setting as kept, then that setting will be returned. Otherwise,
     * this method will raise error.
     *
     * This method is only valid if the device has PJMEDIA_AUD_DEV_CAP_CNG
     * capability in AudioDevInfo.caps flags, otherwise Error will be thrown.
     *
     * @return	    		The audio comfort noise generation feature.
     *
     */
    bool getCng() const throw(Error);

    /**
     * This will configure audio packet loss concealment capability to
     * the sound device being used. If sound device is currently active,
     * the method will forward the setting to the sound device instance
     * to be applied immediately, if it supports it.
     *
     * This method is only valid if the device has PJMEDIA_AUD_DEV_CAP_PLC
     * capability in AudioDevInfo.caps flags, otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the sound device to be
     * used.
     *
     * @param enable		Enable/disable packet loss concealment
     *				feature. Set true to enable.
     * @param keep		Specify whether the setting is to be kept for
     *				future use.
     *
     */
    void setPlc(bool enable, bool keep=true) throw(Error);

    /**
     * Get the audio packet loss concealment capability of the sound device
     * being used. If sound device is currently active, the method will
     * forward the request to the sound device. If sound device is currently
     * inactive, and if application had previously set the setting and mark
     * the setting as kept, then that setting will be returned. Otherwise,
     * this method will raise error.
     *
     * This method is only valid if the device has PJMEDIA_AUD_DEV_CAP_PLC
     * capability in AudioDevInfo.caps flags, otherwise Error will be thrown.
     *
     * @return	    		The audio packet loss concealment feature.
     *
     */
    bool getPlc() const throw(Error);

private:
    AudioDevInfoVector		 audioDevList;
    AudioMedia			*devMedia;

    /**
     * Constructor.
     */
    AudDevManager();

    /**
     * Destructor.
     */
    ~AudDevManager();

    void clearAudioDevList();
    int getActiveDev(bool is_capture) const throw(Error);

    friend class Endpoint;
};


/*************************************************************************
* Video media
*/

/**
 * Representation of media coordinate.
 */
struct MediaCoordinate
{
    int		x;	    /**< X position of the coordinate */
    int		y;	    /**< Y position of the coordinate */
};

/**
 * Representation of media size.
 */
struct MediaSize
{
    unsigned	w;	    /**< The width.	*/
    unsigned 	h;	    /**< The height.	*/
};

/**
 * Window handle.
 */
typedef struct WindowHandle {
    void    	*window;    /**< Window		*/
    void    	*display;   /**< Display	*/
} WindowHandle;

/**
 * Video window handle.
 */
struct VideoWindowHandle
{
    /**
     * The window handle type.
     */
    pjmedia_vid_dev_hwnd_type 	type;

    /**
     * The window handle.
     */
    WindowHandle 		handle;
};

/**
 * This structure describes video window info.
 */
typedef struct VideoWindowInfo
{
    /**
     * Flag to indicate whether this window is a native window,
     * such as created by built-in preview device. If this field is
     * true, only the video window handle field of this
     * structure is valid.
     */
    bool 		isNative;

    /**
     * Video window handle.
     */
    VideoWindowHandle 	winHandle;

    /**
     * Renderer device ID.
     */
    int 		renderDeviceId;

    /**
     * Window show status. The window is hidden if false.
     */
    bool		show;

    /**
     * Window position.
     */
    MediaCoordinate 	pos;

    /**
     * Window size.
     */
    MediaSize 		size;

} VideoWindowInfo;

/**
 * Video window.
 */
class VideoWindow
{
public:
    /**
     * Constructor
     */
    VideoWindow(int win_id);

    /**
     * Get window info.
     *
     * @return			video window info.
     */
    VideoWindowInfo getInfo() const throw(Error);
    
    /**
     * Show or hide window. This operation is not valid for native windows
     * (VideoWindowInfo.isNative=true), on which native windowing API
     * must be used instead.
     *
     * @param show		Set to true to show the window, false to
     * 				hide the window.
     *
     */
    void Show(bool show) throw(Error);
    
    /**
     * Set video window position. This operation is not valid for native windows
     * (VideoWindowInfo.isNative=true), on which native windowing API
     * must be used instead.
     *
     * @param pos		The window position.
     *
     */
    void setPos(const MediaCoordinate &pos) throw(Error);
    
    /**
     * Resize window. This operation is not valid for native windows
     * (VideoWindowInfo.isNative=true), on which native windowing API
     * must be used instead.
     *
     * @param size		The new window size.
     *
     */
    void setSize(const MediaSize &size) throw(Error);
    
    /**
     * Rotate the video window. This function will change the video orientation
     * and also possibly the video window size (width and height get swapped).
     * This operation is not valid for native windows (VideoWindowInfo.isNative
     * =true), on which native windowing API must be used instead.
     *
     * @param angle		The rotation angle in degrees, must be
     *				multiple of 90.
     *				Specify positive value for clockwise rotation or
     *				negative value for counter-clockwise rotation.
     */
    void rotate(int angle) throw(Error);

    /**
     * Set output window. This operation is valid only when the underlying
     * video device supports PJMEDIA_VIDEO_DEV_CAP_OUTPUT_WINDOW capability AND
     * allows the output window to be changed on-the-fly, otherwise Error will
     * be thrown. Currently it is only supported on Android.
     *
     * @param win		The new output window.
     */
    void setWindow(const VideoWindowHandle &win) throw(Error);
    
private:
    pjsua_vid_win_id		winId;
};

/**
 * This structure contains parameters for VideoPreview::start()
 */
struct VideoPreviewOpParam {
    /**
     * Device ID for the video renderer to be used for rendering the
     * capture stream for preview. This parameter is ignored if native
     * preview is being used.
     *
     * Default: PJMEDIA_VID_DEFAULT_RENDER_DEV
     */
    pjmedia_vid_dev_index   rendId;

    /**
     * Show window initially.
     *
     * Default: PJ_TRUE.
     */
    bool		    show;

    /**
     * Window flags.  The value is a bitmask combination of
     * \a pjmedia_vid_dev_wnd_flag.
     *
     * Default: 0.
     */
    unsigned		    windowFlags;

    /**
     * Media format. If left unitialized, this parameter will not be used.
     */
    MediaFormat		    format;

    /**
     * Optional output window to be used to display the video preview.
     * This parameter will only be used if the video device supports
     * PJMEDIA_VID_DEV_CAP_OUTPUT_WINDOW capability and the capability
     * is not read-only.
     */
    VideoWindowHandle	    window;

public:
    /**
     * Default constructor initializes with default values.
     */
    VideoPreviewOpParam();

    /** 
     * Convert from pjsip
     */
    void fromPj(const pjsua_vid_preview_param &prm);

    /**
     * Convert to pjsip
     */
    pjsua_vid_preview_param toPj() const;
};

/**
 * Video Preview
 */
class VideoPreview {
public:
    /**
     * Constructor
     */
    VideoPreview(int dev_id);

    /**
     * Determine if the specified video input device has built-in native
     * preview capability. This is a convenience function that is equal to
     * querying device's capability for PJMEDIA_VID_DEV_CAP_INPUT_PREVIEW
     * capability.
     *
     * @return		true if it has.
     */
    bool hasNative();

    /**
     * Start video preview window for the specified capture device.
     *
     * @param p		Video preview parameters. 
     */
    void start(const VideoPreviewOpParam &param) throw(Error);

    /**
     * Stop video preview.
     */
    void stop() throw(Error);

    /*
     * Get the preview window handle associated with the capture device,if any.
     */
    VideoWindow getVideoWindow();

private:
    pjmedia_vid_dev_index devId;
};

/**
 * Video device information structure.
 */
struct VideoDevInfo
{
    /**
     * The device ID
     */
    pjmedia_vid_dev_index id;

    /**
     * The device name
     */
    string name;

    /**
     * The underlying driver name
     */
    string driver;

    /**
     * The supported direction of the video device, i.e. whether it supports
     * capture only, render only, or both.
     */
    pjmedia_dir dir;

    /** 
     * Device capabilities, as bitmask combination of #pjmedia_vid_dev_cap 
     */
    unsigned caps;

    /**
     * Array of supported video formats. Some fields in each supported video
     * format may be set to zero or of "unknown" value, to indicate that the
     * value is unknown or should be ignored. When these value are not set
     * to zero, it indicates that the exact format combination is being used.
     */
    MediaFormatVector fmt;

    /**
     * Construct from pjmedia_vid_dev_info.
     */
    void fromPj(const pjmedia_vid_dev_info &dev_info);

    /**
     * Destructor.
     */
    ~VideoDevInfo();
};

/** Array of video device info */
typedef std::vector<VideoDevInfo*> VideoDevInfoVector;

/**
 * Parameter for switching device with PJMEDIA_VID_DEV_CAP_SWITCH capability.
 */
struct VideoSwitchParam
{
    /**
     * Target device ID to switch to. Once the switching is successful, the
     * video stream will use this device and the old device will be closed.
     */
    pjmedia_vid_dev_index target_id;
};
 
/**
 * Video device manager.
 */
class VidDevManager {
public:
    /**
     * Refresh the list of video devices installed in the system. This function
     * will only refresh the list of video device so all active video streams
     * will be unaffected. After refreshing the device list, application MUST
     * make sure to update all index references to video devices (i.e. all
     * variables of type pjmedia_vid_dev_index) before calling any function
     * that accepts video device index as its parameter.
     */
    void refreshDevs() throw(Error);

    /**
     * Get the number of video devices installed in the system.
     *
     * @return		The number of devices.
     */
    unsigned getDevCount();

    /**
     * Retrieve the video device info for the specified device index.     
     *
     * @param dev_id	The video device id
     * 
     * @return		The list of video device info
     */
    VideoDevInfo getDevInfo(int dev_id) const throw(Error);

    /**
     * Enum all video devices installed in the system.
     *
     * @return		The list of video device info
     */
    const VideoDevInfoVector &enumDev() throw(Error);

    /**
     * Lookup device index based on the driver and device name.
     *
     * @param drv_name	The driver name.
     * @param dev_name	The device name.
     *
     * @return		The device ID. If the device is not found, 
     *			Error will be thrown.
     */
    int lookupDev(const string &drv_name,
		  const string &dev_name) const throw(Error);

    /**
     * Get string info for the specified capability.
     *
     * @param cap	The capability ID.
     *
     * @return		Capability name.
     */
    string capName(pjmedia_vid_dev_cap cap) const;

    /**
     * This will configure video format capability to the video device. 
     * If video device is currently active, the method will forward the setting 
     * to the video device instance to be applied immediately, if it 
     * supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_VID_DEV_CAP_FORMAT capability in VideoDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the video device to be
     * used.
     *
     * @param dev_id	The video device id.	
     * @param format	The video format.
     * @param keep	Specify whether the setting is to be kept for
     * 			future use.
     */
    void setFormat(int dev_id, 
		   const MediaFormatVideo &format, 
		   bool keep) throw(Error);

    /**
     * Get the video format capability to the video device.
     * If video device is currently active, the method will forward the request
     * to the video device. If video device is currently inactive, and if 
     * application had previously set the setting and mark the setting as kept, 
     * then that setting will be returned. Otherwise, this method will 
     * raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_VID_DEV_CAP_FORMAT capability in VideoDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * @param dev_id	The video device id.
     * @return keep	The video format.
     */
    MediaFormatVideo getFormat(int dev_id) const throw(Error);

    /**
     * This will configure video format capability to the video device.
     * If video device is currently active, the method will forward the setting
     * to the video device instance to be applied immediately, if it
     * supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_VID_DEV_CAP_INPUT_SCALE capability in VideoDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the video device to be
     * used.
     *
     * @param dev_id	The video device id.
     * @param scale	The video scale.
     * @param keep	Specify whether the setting is to be kept for
     * 			future use.
     */
    void setInputScale(int dev_id, 
		       const MediaSize &scale, 
		       bool keep) throw(Error);

    /**
     * Get the video input scale capability to the video device.
     * If video device is currently active, the method will forward the request
     * to the video device. If video device is currently inactive, and if
     * application had previously set the setting and mark the setting as kept,
     * then that setting will be returned. Otherwise, this method will
     * raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_VID_DEV_CAP_FORMAT capability in VideoDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * @param dev_id	The video device id.
     * @return keep	The video format.
     */
    MediaSize getInputScale(int dev_id) const throw(Error);

    /**
     * This will configure fast switching to another video device.
     * If video device is currently active, the method will forward the setting
     * to the video device instance to be applied immediately, if it
     * supports it.
     *
     * This method is only valid if the device has 
     * PJMEDIA_VID_DEV_CAP_OUTPUT_WINDOW_FLAGS capability in VideoDevInfo.caps 
     * flags, otherwise Error will be thrown.
     * 
     * Note that in case the setting is kept for future use, it will be applied
     * to any devices, even when application has changed the video device to be
     * used.
     *
     * @param dev_id	The video device id.
     * @param flags	The video window flag.
     * @param keep	Specify whether the setting is to be kept for
     * 			future use.
     */
    void setOutputWindowFlags(int dev_id, int flags, bool keep) throw(Error);
    
    /**
     * Get the window output flags capability to the video device.
     * If video device is currently active, the method will forward the request
     * to the video device. If video device is currently inactive, and if
     * application had previously set the setting and mark the setting as kept,
     * then that setting will be returned. Otherwise, this method will
     * raise error.
     *
     * This method is only valid if the device has
     * PJMEDIA_VID_DEV_CAP_OUTPUT_WINDOW_FLAGS capability in VideoDevInfo.caps 
     * flags, otherwise Error will be thrown.
     *
     * @param dev_id	The video device id.
     * @return keep	The video format.
     */
    int getOutputWindowFlags(int dev_id) throw(Error);

    /**
     * This will configure fast switching to another video device.
     * If video device is currently active, the method will forward the setting
     * to the video device instance to be applied immediately, if it
     * supports it.
     *
     * This method is only valid if the device has
     * PJMEDIA_VID_DEV_CAP_SWITCH capability in VideoDevInfo.caps flags,
     * otherwise Error will be thrown.
     *
     * @param dev_id	The video device id.
     * @param param	The video switch param.
     */
    void switchDev(int dev_id,
		   const VideoSwitchParam &param) throw(Error);

    /**
     * Check whether the video capture device is currently active, i.e. if
     * a video preview has been started or there is a video call using
     * the device.    
     *
     * @param dev_id	The video device id
     * 
     * @return		True if it's active.
     */
    bool isCaptureActive(int dev_id) const;

    /**
     * This will configure video orientation of the video capture device.
     * If the device is currently active (i.e. if there is a video call
     * using the device or a video preview has been started), the method
     * will forward the setting to the video device instance to be applied
     * immediately, if it supports it.
     *
     * The setting will be saved for future opening of the video device,
     * if the "keep" argument is set to true. If the video device is
     * currently inactive, and the "keep" argument is false, this method
     * will throw Error.
     *
     * @param dev_id	The video device id
     * @param orient	The video orientation.
     * @param keep	Specify whether the setting is to be kept for
     * 			future use.
     *
     */
    void setCaptureOrient(pjmedia_vid_dev_index dev_id,
    			  pjmedia_orient orient,
    			  bool keep=true) throw(Error);

private:
    VideoDevInfoVector videoDevList;

    void clearVideoDevList();

    /**
     * Constructor.
     */
    VidDevManager();

    /**
     * Destructor.
     */
    ~VidDevManager();

    friend class Endpoint;
};


/*************************************************************************
* Codec management
*/

/**
 * This structure describes codec information.
 */
struct CodecInfo
{
    /**
     * Codec unique identification.
     */
    string		codecId;

    /**
     * Codec priority (integer 0-255).
     */
    pj_uint8_t		priority;

    /**
     * Codec description.
     */
    string		desc;

    /**
     * Construct from pjsua_codec_info.
     */
    void fromPj(const pjsua_codec_info &codec_info);
};

/** Array of codec info */
typedef std::vector<CodecInfo*> CodecInfoVector;

/**
 * Structure of codec specific parameters which contains name=value pairs.
 * The codec specific parameters are to be used with SDP according to
 * the standards (e.g: RFC 3555) in SDP 'a=fmtp' attribute.
 */
typedef struct CodecFmtp
{
    string name;
    string val;
} CodecFmtp;

/** Array of codec fmtp */
typedef std::vector<CodecFmtp> CodecFmtpVector;

/**
 * Audio codec parameters info.
 */
struct CodecParamInfo
{
    unsigned	clockRate;		/**< Sampling rate in Hz	    */
    unsigned	channelCnt;		/**< Channel count.		    */
    unsigned 	avgBps;			/**< Average bandwidth in bits/sec  */
    unsigned	maxBps;			/**< Maximum bandwidth in bits/sec  */
    unsigned    maxRxFrameSize;		/**< Maximum frame size             */
    unsigned 	frameLen;		/**< Decoder frame ptime in msec.   */
    unsigned  	pcmBitsPerSample;	/**< Bits/sample in the PCM side    */
    unsigned  	pt;			/**< Payload type.		    */
    pjmedia_format_id fmtId;		/**< Source format, it's format of
					     encoder input and decoder
					     output.			    */
};

/**
 * Audio codec parameters setting.
 */
struct CodecParamSetting
{
    unsigned  	frmPerPkt;	    /**< Number of frames per packet.	*/
    bool	vad;		    /**< Voice Activity Detector.	*/
    bool	cng;		    /**< Comfort Noise Generator.	*/
    bool	penh;		    /**< Perceptual Enhancement		*/
    bool	plc;		    /**< Packet loss concealment	*/
    bool	reserved;	    /**< Reserved, must be zero.	*/
    CodecFmtpVector encFmtp;	    /**< Encoder's fmtp params.		*/
    CodecFmtpVector decFmtp;	    /**< Decoder's fmtp params.		*/
};

/**
 * Detailed codec attributes used in configuring an audio codec and in querying
 * the capability of audio codec factories.
 *
 * Please note that codec parameter also contains SDP specific setting,
 * #setting::decFmtp and #setting::encFmtp, which may need to be set 
 * appropriately based on the effective setting. 
 * See each codec documentation for more detail.
 */
struct CodecParam
{
    struct CodecParamInfo info;
    struct CodecParamSetting setting;

    void fromPj(const pjmedia_codec_param &param);

    pjmedia_codec_param toPj() const;
};

/**
 * Detailed codec attributes used in configuring a video codec and in querying
 * the capability of video codec factories. 
 *
 * Please note that codec parameter also contains SDP specific setting,
 * #decFmtp and #encFmtp, which may need to be set appropriately based on
 * the effective setting. See each codec documentation for more detail.
 */
struct VidCodecParam
{
    pjmedia_dir         dir;            /**< Direction                      */
    pjmedia_vid_packing packing; 	/**< Packetization strategy.	    */

    struct
    MediaFormatVideo    encFmt;         /**< Encoded format	            */
    CodecFmtpVector	encFmtp;        /**< Encoder fmtp params	    */
    unsigned            encMtu;         /**< MTU or max payload size setting*/

    struct
    MediaFormatVideo    decFmt;         /**< Decoded format	            */
    CodecFmtpVector	decFmtp;        /**< Decoder fmtp params	    */

    bool		ignoreFmtp;	/**< Ignore fmtp params. If set to
					     true, the codec will apply
					     format settings specified in
					     encFmt and decFmt only.	    */

    void fromPj(const pjmedia_vid_codec_param &param);

    pjmedia_vid_codec_param toPj() const;
};


/**
 * @}  // PJSUA2_MED
 */

} // namespace pj

#endif	/* __PJSUA2_MEDIA_HPP__ */
