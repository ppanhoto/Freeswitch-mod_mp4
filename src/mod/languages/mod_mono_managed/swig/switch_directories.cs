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

public class switch_directories : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal switch_directories(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(switch_directories obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~switch_directories() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        freeswitchPINVOKE.delete_switch_directories(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public string base_dir {
    set {
      freeswitchPINVOKE.switch_directories_base_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_base_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string mod_dir {
    set {
      freeswitchPINVOKE.switch_directories_mod_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_mod_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string conf_dir {
    set {
      freeswitchPINVOKE.switch_directories_conf_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_conf_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string log_dir {
    set {
      freeswitchPINVOKE.switch_directories_log_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_log_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string db_dir {
    set {
      freeswitchPINVOKE.switch_directories_db_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_db_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string script_dir {
    set {
      freeswitchPINVOKE.switch_directories_script_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_script_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string temp_dir {
    set {
      freeswitchPINVOKE.switch_directories_temp_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_temp_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string htdocs_dir {
    set {
      freeswitchPINVOKE.switch_directories_htdocs_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_htdocs_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string grammar_dir {
    set {
      freeswitchPINVOKE.switch_directories_grammar_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_grammar_dir_get(swigCPtr);
      return ret;
    } 
  }

  public string storage_dir {
    set {
      freeswitchPINVOKE.switch_directories_storage_dir_set(swigCPtr, value);
    } 
    get {
      string ret = freeswitchPINVOKE.switch_directories_storage_dir_get(swigCPtr);
      return ret;
    } 
  }

  public switch_directories() : this(freeswitchPINVOKE.new_switch_directories(), true) {
  }

}

}
