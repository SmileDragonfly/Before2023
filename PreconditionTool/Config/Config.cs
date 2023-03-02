using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using TMSLog;
using TMSPreconditionTool.Tool;

namespace TMSPreconditionTool.Config
{
    public enum TypeCheck
    {
        Application = 0,
        Service,
        BatFile
    }
    public enum Executed
    {
        No,
        Yes
    }
    public class PreconditionConfig
    {
        public string configTag { get; set; }
        public string displayName { get; set; }
        public string checkName { get; set; }
        public string typeCheck { get; set; }
        public string version { get; set; }
        public string versionCondition { get; set; }
        public string path { get; set; }
        public string command { get; set; }
        public string executed { get; set; }
        public int estimateTime { get; set; }
        public int timeout { get; set; }
        public bool isSatify { get; set; }
    }
    public class Config
    {
        private static readonly ILog _logger = LogUtils.GetLogger();
        private const string configFile = "\\Config.ini";
        private const string numberPreconditionTag = "NumberPrecondition";
        private const string preconditionPrefixTag = "Precondition";

        public int numberPrecondition;
        public List<PreconditionConfig> listPreconditions;
        private IniFile iniFile { get; }

        public Config(String sConfigFolder)
        {
            try
            {
                _logger.Info("Read Configuration ....");
                iniFile = new IniFile(sConfigFolder + configFile);
                listPreconditions = new List<PreconditionConfig>();

                numberPrecondition = Int32.Parse(iniFile.IniReadValue(numberPreconditionTag, "NUMBER"));
                for(int i = 0; i < numberPrecondition; i++)
                {
                    PreconditionConfig item = new PreconditionConfig();
                    String sTag = preconditionPrefixTag + (i + 1);
                    item.configTag = sTag;
                    item.displayName = iniFile.IniReadValue(sTag, "DisplayName");
                    item.checkName = iniFile.IniReadValue(sTag, "CheckName");
                    item.typeCheck = iniFile.IniReadValue(sTag, "TypeCheck");
                    item.version = iniFile.IniReadValue(sTag, "Version");
                    item.versionCondition = iniFile.IniReadValue(sTag, "VersionCondition");
                    item.path = iniFile.IniReadValue(sTag, "Path");
                    item.command = iniFile.IniReadValue(sTag, "Command");
                    item.executed = iniFile.IniReadValue(sTag, "Executed");
                    item.estimateTime = Convert.ToInt32(iniFile.IniReadValue(sTag, "EstimateTime"));
                    item.timeout = Convert.ToInt32(iniFile.IniReadValue(sTag, "Timeout"));
                    listPreconditions.Add(item);
                }
            }
            catch (Exception ex)
            { 
                _logger.Error($"Exception Config", ex);
            }
        }

        public void UpdateExecutedField(string sTag, string sValue)
        {
            if(iniFile != null)
            {
                _logger.Info($"Update executed field {sTag}-{sValue}");
                iniFile.WriteIniValue(sTag, "Executed", sValue);
            }
            else
            {
                _logger.Info($"Cannot not update executed field {sTag}-{sValue}");
            }
        }
    }
}
