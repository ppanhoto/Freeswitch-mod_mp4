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

public class switch_scheduler_task : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal switch_scheduler_task(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(switch_scheduler_task obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~switch_scheduler_task() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        freeswitchPINVOKE.delete_switch_scheduler_task(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public long created {
    set {
      freeswitchPINVOKE.switch_scheduler_task_created_set(swigCPtr, value);
    } 
    get {
      long ret = freeswitchPINVOKE.switch_scheduler_task_created_get(swigCPtr);
      return ret;
    } 
  }

  public long runtime {
    set {
      freeswitchPINVOKE.switch_scheduler_task_runtime_set(swigCPtr, value);
    } 
    get {
      long ret = freeswitchPINVOKE.switch_scheduler_task_runtime_get(swigCPtr);
      return ret;
    } 
  }

  public uint cmd_id {
    set {
      freeswitchPINVOKE.switch_scheduler_task_cmd_id_set(swigCPtr, value);
    } 
    get {
      uint ret = freeswitchPINVOKE.switch_scheduler_task_cmd_id_get(swigCPtr);
      return ret;
    } 
  }

  public string group {
    set {
      freeswitchPINVOKE.switch_scheduler_task_group_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_scheduler_task_group_get(swigCPtr);
      return ret;
    } 
  }

  public SWIGTYPE_p_void cmd_arg {
    set {
      freeswitchPINVOKE.switch_scheduler_task_cmd_arg_set(swigCPtr, SWIGTYPE_p_void.getCPtr(value));
    } 
    get {
      IntPtr cPtr = freeswitchPINVOKE.switch_scheduler_task_cmd_arg_get(swigCPtr);
      SWIGTYPE_p_void ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_void(cPtr, false);
      return ret;
    } 
  }

  public uint task_id {
    set {
      freeswitchPINVOKE.switch_scheduler_task_task_id_set(swigCPtr, value);
    } 
    get {
      uint ret = freeswitchPINVOKE.switch_scheduler_task_task_id_get(swigCPtr);
      return ret;
    } 
  }

  public switch_scheduler_task() : this(freeswitchPINVOKE.new_switch_scheduler_task(), true) {
  }

}

}
