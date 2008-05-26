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

public class switch_io_event_hook_recv_dtmf : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal switch_io_event_hook_recv_dtmf(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(switch_io_event_hook_recv_dtmf obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~switch_io_event_hook_recv_dtmf() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        freeswitchPINVOKE.delete_switch_io_event_hook_recv_dtmf(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public SWIGTYPE_p_f_p_switch_core_session_p_q_const__switch_dtmf_t_enum_switch_dtmf_direction_t__switch_status_t recv_dtmf {
    set {
      freeswitchPINVOKE.switch_io_event_hook_recv_dtmf_recv_dtmf_set(swigCPtr, SWIGTYPE_p_f_p_switch_core_session_p_q_const__switch_dtmf_t_enum_switch_dtmf_direction_t__switch_status_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_io_event_hook_recv_dtmf_recv_dtmf_get(swigCPtr);
      SWIGTYPE_p_f_p_switch_core_session_p_q_const__switch_dtmf_t_enum_switch_dtmf_direction_t__switch_status_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_p_switch_core_session_p_q_const__switch_dtmf_t_enum_switch_dtmf_direction_t__switch_status_t(cPtr, false);
      return ret;
    } 
  }

  public switch_io_event_hook_recv_dtmf next {
    set {
      freeswitchPINVOKE.switch_io_event_hook_recv_dtmf_next_set(swigCPtr, switch_io_event_hook_recv_dtmf.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_io_event_hook_recv_dtmf_next_get(swigCPtr);
      switch_io_event_hook_recv_dtmf ret = (cPtr == IntPtr.Zero) ? null : new switch_io_event_hook_recv_dtmf(cPtr, false);
      return ret;
    } 
  }

  public switch_io_event_hook_recv_dtmf() : this(freeswitchPINVOKE.new_switch_io_event_hook_recv_dtmf(), true) {
  }

}

}
