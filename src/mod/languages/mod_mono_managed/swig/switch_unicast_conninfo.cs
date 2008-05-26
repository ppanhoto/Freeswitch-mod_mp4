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

public class switch_unicast_conninfo : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal switch_unicast_conninfo(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(switch_unicast_conninfo obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~switch_unicast_conninfo() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        freeswitchPINVOKE.delete_switch_unicast_conninfo(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public SWIGTYPE_p_switch_core_session session {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_session_set(swigCPtr, SWIGTYPE_p_switch_core_session.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_session_get(swigCPtr);
      SWIGTYPE_p_switch_core_session ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_switch_core_session(cPtr, false);
      return ret;
    } 
  }

  public switch_codec read_codec {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_read_codec_set(swigCPtr, switch_codec.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_read_codec_get(swigCPtr);
      switch_codec ret = (cPtr == IntPtr.Zero) ? null : new switch_codec(cPtr, false);
      return ret;
    } 
  }

  public switch_frame write_frame {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_write_frame_set(swigCPtr, switch_frame.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_write_frame_get(swigCPtr);
      switch_frame ret = (cPtr == IntPtr.Zero) ? null : new switch_frame(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_unsigned_char write_frame_data {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_write_frame_data_set(swigCPtr, SWIGTYPE_p_unsigned_char.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_write_frame_data_get(swigCPtr);
      SWIGTYPE_p_unsigned_char ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_unsigned_char(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_switch_socket_t socket {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_socket_set(swigCPtr, SWIGTYPE_p_switch_socket_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_socket_get(swigCPtr);
      SWIGTYPE_p_switch_socket_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_switch_socket_t(cPtr, false);
      return ret;
    } 
  }

  public string local_ip {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_local_ip_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_unicast_conninfo_local_ip_get(swigCPtr);
      return ret;
    } 
  }

  public ushort local_port {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_local_port_set(swigCPtr, value);
    } 
    get {
      ushort ret = freeswitchPINVOKE.switch_unicast_conninfo_local_port_get(swigCPtr);
      return ret;
    } 
  }

  public string remote_ip {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_remote_ip_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_unicast_conninfo_remote_ip_get(swigCPtr);
      return ret;
    } 
  }

  public ushort remote_port {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_remote_port_set(swigCPtr, value);
    } 
    get {
      ushort ret = freeswitchPINVOKE.switch_unicast_conninfo_remote_port_get(swigCPtr);
      return ret;
    } 
  }

  public SWIGTYPE_p_switch_sockaddr_t local_addr {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_local_addr_set(swigCPtr, SWIGTYPE_p_switch_sockaddr_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_local_addr_get(swigCPtr);
      SWIGTYPE_p_switch_sockaddr_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_switch_sockaddr_t(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_switch_sockaddr_t remote_addr {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_remote_addr_set(swigCPtr, SWIGTYPE_p_switch_sockaddr_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_remote_addr_get(swigCPtr);
      SWIGTYPE_p_switch_sockaddr_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_switch_sockaddr_t(cPtr, false);
      return ret;
    } 
  }

  public SWIGTYPE_p_switch_mutex_t flag_mutex {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_flag_mutex_set(swigCPtr, SWIGTYPE_p_switch_mutex_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_unicast_conninfo_flag_mutex_get(swigCPtr);
      SWIGTYPE_p_switch_mutex_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_switch_mutex_t(cPtr, false);
      return ret;
    } 
  }

  public int flags {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_flags_set(swigCPtr, value);
    } 
    get {
      int ret = freeswitchPINVOKE.switch_unicast_conninfo_flags_get(swigCPtr);
      return ret;
    } 
  }

  public int type {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_type_set(swigCPtr, value);
    } 
    get {
      int ret = freeswitchPINVOKE.switch_unicast_conninfo_type_get(swigCPtr);
      return ret;
    } 
  }

  public int transport {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_transport_set(swigCPtr, value);
    } 
    get {
      int ret = freeswitchPINVOKE.switch_unicast_conninfo_transport_get(swigCPtr);
      return ret;
    } 
  }

  public int stream_id {
    set {
      freeswitchPINVOKE.switch_unicast_conninfo_stream_id_set(swigCPtr, value);
    } 
    get {
      int ret = freeswitchPINVOKE.switch_unicast_conninfo_stream_id_get(swigCPtr);
      return ret;
    } 
  }

  public switch_unicast_conninfo() : this(freeswitchPINVOKE.new_switch_unicast_conninfo(), true) {
  }

}

}
