/*
    BSD 3-Clause License

    Copyright (c) 2018, KORG INC.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*/

/**
 * @file    osc_api.h
 * @brief   Oscillator runtime API.
 *
 * @addtogroup osc Oscillator
 * @{
 *
 * @addtogroup osc_api Runtime API
 * @{
 */

#ifndef __osc_api_h
#define __osc_api_h


#ifdef __cplusplus
extern "C" {
#endif

#define __fast_inline static inline __attribute__((always_inline, optimize("Ofast")))  
  
  /*===========================================================================*/
  /* Runtime Environment                                                       */
  /*===========================================================================*/

  /**
   * @name   Runtime Environment
   * @{ 
   */

  /**
   * Current platform
   */
  extern const uint32_t k_osc_api_platform;
  
  /**
   * Current API version
   */
  extern const uint32_t k_osc_api_version;
  
  /**
   * Get MCU hash
   *
   * @return  A MCU specific "unique" hash.
   */
  uint32_t _osc_mcu_hash(void);

  __fast_inline uint32_t osc_mcu_hash(void) {
    return _osc_mcu_hash();
  }

#define k_samplerate        (48000)
#define k_samplerate_recipf (2.08333333333333e-005f)

  /** @} */
  
  /*===========================================================================*/
  /* Lookup tables                                                             */
  /*===========================================================================*/

  /**
   * @name   Note to frequency conversion table
   * @{ 
   */
  
#define k_midi_to_hz_size      (152)
  
  extern const float midi_to_hz_lut_f[k_midi_to_hz_size];

#define k_note_mod_fscale      (0.00392156862745098f)
#define k_note_max_hz          (23679.643054f)

/** Clip upper bound of x to m (inclusive)
 */
__fast_inline float clipmaxf(const float x, const float m)
{
    return (x < m) ? x : m;
}

/** Clip upper bound of unsigned integer x to m (inclusive)
 */
static inline __attribute__((optimize("Ofast"), always_inline))
uint32_t clipmaxu32(const uint32_t x, const uint32_t m) {
  return (((x)>=m)?m:(x));
}

/** Linear interpolation
 */
__fast_inline float linintf(const float fr, const float x0, const float x1)
{
    return x0 + fr * (x1 - x0);
}


  
  /**
   * Get Hertz value for note
   *
   * @param note Note in [0-151] range.
   * @return     Corresponding Hertz value.
   */
  __fast_inline float osc_notehzf(uint8_t note) {
    return midi_to_hz_lut_f[clipmaxu32(note,k_midi_to_hz_size-1)];
  }

  /**
   * Get floating point phase increment for given note and fine modulation
   *
   * @param note Note in [0-151] range, mod in [0-255] range.
   * @return     Corresponding 0-1 phase increment in floating point.
   */
  __fast_inline float osc_w0f_for_note(uint8_t note, uint8_t mod) {    
    const float f0 = osc_notehzf(note);
    const float f1 = osc_notehzf(note+1);
    
    const float f = clipmaxf(linintf(mod * k_note_mod_fscale, f0, f1), k_note_max_hz);

    return f * k_samplerate_recipf;
  }

#ifdef __cplusplus
}
#endif 


#endif // __osc_api_h

/** @} @} */
