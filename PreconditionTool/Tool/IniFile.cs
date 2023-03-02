using System.Runtime.InteropServices;
using System.Text;

namespace TMSPreconditionTool.Tool
{
    /// <summary>
    /// Class for interaction with INI file
    /// </summary>
    public class IniFile
    {
        #region -- INI --

        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal,
            int size, string filePath);

        #endregion

        #region -- Private members --
        private string _iniPath;
        #endregion

        #region -- Public Properties --
        public string IniPath
        {
            set { _iniPath = value; }
        }

        #endregion

        #region 
        public IniFile(string iniPath)
        {
            _iniPath = iniPath;
        }

        #endregion

        #region -- Public Methods --
        public void WriteIniValue(string section, string key, object value)
        {
            WritePrivateProfileString(section, key, value.ToString(), _iniPath);
        }

        public string IniReadValue(string section, string key)
        {
            StringBuilder temp = new StringBuilder(2047);
            GetPrivateProfileString(section, key, "", temp, temp.Capacity, _iniPath);
            return temp.ToString();
        }
        #endregion
    }
}
