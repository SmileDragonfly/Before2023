using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using TMSPreconditionTool.Config;
using TMSPreconditionTool.Tool;

namespace TestPreconditionTool
{
    [TestClass]
    public class ConfigTest
    {
        [TestMethod]
        public void TestLoadConfig()
        {
            Config cfg = new Config(@"D:\VSProject\CSharp2022\TMSPreconditionTool\bin\Debug");
            IniFile iniFile = new IniFile(@"D:\VSProject\CSharp2022\TMSPreconditionTool\bin\Debug\Config.ini");
            Assert.AreEqual(14, cfg.numberPrecondition);
        }
        [TestMethod]
        public void TestCheckAppInstalled()
        {
            bool bCheck = RegistryAccess.IsAppInstalled("Microsoft ASP.NET MVC 4", "4.0.20714.0", "Equal");
            Assert.AreEqual(true, bCheck);
        }
    }
}
