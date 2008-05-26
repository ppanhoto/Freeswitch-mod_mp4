/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.35
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace FreeSWITCH.Native {

using System;
using System.Runtime.InteropServices;

public class switch_speech_handle : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal switch_speech_handle(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(switch_speech_handle obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~switch_speech_handle() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        freeswitchPINVOKE.delete_switch_speech_handle(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public switch_speech_interface speech_interface {
    set {
      freeswitchPINVOKE.switch_speech_handle_speech_interface_set(swigCPtr, switch_speech_interface.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_speech_handle_speech_interface_get(swigCPtr);
      switch_speech_interface ret = (cPtr == IntPtr.Zero) ? null : new switch_speech_interface(cPtr, false);
      return ret;
    } 
  }

  public uint flags {
    set {
      freeswitchPINVOKE.switch_speech_handle_flags_set(swigCPtr, value);
    } 
    get {
      uint ret = freeswitchPINVOKE.switch_speech_handle_flags_get(swigCPtr);
      return ret;
    } 
  }

  public string name {
    set {
      freeswitchPINVOKE.switch_speech_handle_name_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_speech_handle_name_get(swigCPtr);
      return ret;
    } 
  }

  public uint rate {
    set {
      freeswitchPINVOKE.switch_speech_handle_rate_set(swigCPtr, value);
    } 
    get {
      uint ret = freeswitchPINVOKE.switch_speech_handle_rate_get(swigCPtr);
      return ret;
    } 
  }

  public uint speed {
    set {
      freeswitchPINVOKE.switch_speech_handle_speed_set(swigCPtr, value);
    } 
    get {
      uint ret = freeswitchPINVOKE.switch_speech_handle_speed_get(swigCPtr);
      return ret;
    } 
  }

  public uint samples {
    set {
      freeswitchPINVOKE.switch_speech_handle_samples_set(swigCPtr, value);
    } 
    get {
      uint ret = freeswitchPINVOKE.switch_speech_handle_samples_get(swigCPtr);
      return ret;
    } 
  }

  public string voice {
    set {
      freeswitchPINVOKE.switch_speech_handle_voice_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_speech_handle_voice_get(swigCPtr);
      return ret;
    } 
  }

  public string engine {
    set {
      freeswitchPINVOKE.switch_speech_handle_engine_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_speech_handle_engine_get(swigCPtr);
      return ret;
    } 
  }

  public string param {
    set {
      freeswitchPINVOKE.switch_speech_handle_param_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_speech_handle_param_get(swigCPtr);
      return ret;
    } 
  }

  public SWIGTYPE_p_apr_pool_t memory_pool {
    set {
      freeswitchPINVOKE.switch_speech_handle_memory_pool_set(swigCPtr, SWIGTYPE_p_apr_pool_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_speech_handle_memory_pool_get(swigCPtr);
      SWIGTYPE_p_apr_pool_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_apr_pool_t(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_void private_info {
    set {
      freeswitchPINVOKE.switch_speech_handle_private_info_set(swigCPtr, SWIGTYPE_p_void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_speech_handle_private_info_get(swigCPtr);
      SWIGTYPE_p_void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_void(cPtr, false);
      return ret;
    } 
  }

  public switch_speech_handle() : this(freeswitchPINVOKE.new_switch_speech_handle(), true) {
  }

}

}
