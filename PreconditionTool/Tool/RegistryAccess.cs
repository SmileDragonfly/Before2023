using log4net;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TMSLog;

namespace TMSPreconditionTool.Tool
{
    public enum VersionCondition
    {
        LessThan = -1,
        Equal,
        GreaterThan
    }
    public class ApplicationInfomation
    {
        public string displayName { get; set; }
        public string displayVersion { get; set; }
    }
    /// <summary>
    ///     Class use to get specific informations in registry
    /// </summary>
    public static class RegistryAccess
    {
        private static readonly ILog _logger = LogUtils.GetLogger();
        /// <summary>
        /// Get all installed apps in registry64 and registry32
        /// </summary>
        /// <returns></returns>
        public static List<ApplicationInfomation> GetAllAppInfo()
        {
            List<ApplicationInfomation>  listApplications = new List<ApplicationInfomation>();
            try
            {
                using (var localMachine = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64))
                {
                    string registry_key = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall";
                    using (Microsoft.Win32.RegistryKey key = localMachine.OpenSubKey(registry_key))
                    {
                        foreach (string subkey_name in key.GetSubKeyNames())
                        {
                            using (RegistryKey subkey = key.OpenSubKey(subkey_name))
                            {
                                ApplicationInfomation appItem = new ApplicationInfomation();
                                if (subkey.GetValue("DisplayName") != null)
                                {
                                    appItem.displayName = subkey.GetValue("DisplayName").ToString(); ;
                                }
                                else
                                {
                                    continue;
                                }
                                if (subkey.GetValue("DisplayVersion") != null)
                                {
                                    appItem.displayVersion = subkey.GetValue("DisplayVersion").ToString();
                                }
                                listApplications.Add(appItem);
                            }
                        }
                    }
                }
                using (var localMachine = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry32))
                {
                    string registry_key = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall";
                    using (Microsoft.Win32.RegistryKey key = localMachine.OpenSubKey(registry_key))
                    {
                        foreach (string subkey_name in key.GetSubKeyNames())
                        {
                            using (RegistryKey subkey = key.OpenSubKey(subkey_name))
                            {
                                ApplicationInfomation appItem = new ApplicationInfomation();
                                if (subkey.GetValue("DisplayName") != null)
                                {
                                    appItem.displayName = subkey.GetValue("DisplayName").ToString(); ;
                                }
                                else
                                {
                                    continue;
                                }
                                if (subkey.GetValue("DisplayVersion") != null)
                                {
                                    appItem.displayVersion = subkey.GetValue("DisplayVersion").ToString();
                                }
                                listApplications.Add(appItem);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                _logger.Error("Exception GetAllAppInfo", ex);
            }
            return listApplications;
        }

        /// <summary>
        /// Check a app installed or not with correct version
        /// </summary>
        /// <param name="sAppName"></param>
        /// <param name="sVersion"></param>
        /// <returns></returns>
        public static bool IsAppInstalled(string sAppName, string sVersion, string versionCondition)
        {
            List<ApplicationInfomation> listApplications = GetAllAppInfo();
            var appVersion = new Version(sVersion);
            foreach (ApplicationInfomation item in listApplications)
            {
                if (item.displayName.Contains(sAppName))
                {
                    var tempVersion = new Version(item.displayVersion);
                    if (versionCondition == VersionCondition.Equal.ToString())
                    {
                        if(tempVersion.CompareTo(appVersion) == 0)
                        {
                            return true;
                        }
                    }
                    else if (versionCondition == VersionCondition.GreaterThan.ToString())
                    {
                        if (tempVersion.CompareTo(appVersion) >= 0)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if (tempVersion.CompareTo(appVersion) <= 0)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
    }
}
