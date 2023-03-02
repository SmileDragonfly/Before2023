using log4net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using TMSLog;

namespace TMSPreconditionTool.Tool
{
    /// <summary>
    /// Struct contain service informations
    /// </summary>
    public class ServiceInformation
    {
        public string name { get; set; }
        public string status { get; set; }
    }
    /// <summary>
    /// Class use to get specific information about services
    /// </summary>
    public static class ServiceAccess
    {
        private static readonly ILog _logger = LogUtils.GetLogger();
        /// <summary>
        /// Get all services name and status in ServiceManager
        /// </summary>
        /// <returns></returns>
        public static List<ServiceInformation> GetAllServicesInfo()
        {
            List<ServiceInformation> listServices = new List<ServiceInformation>();
            try
            {
                ServiceController[] services = ServiceController.GetServices();
                foreach (ServiceController item in services)
                {
                    ServiceInformation siItem = new ServiceInformation();
                    siItem.name = item.DisplayName;
                    siItem.status = item.Status.ToString();
                    listServices.Add(siItem);
                }
            }
            catch (Exception ex)
            {
                _logger.Error("Exception GetAllServicesInfo", ex);
            }
            return listServices;
        }

        /// <summary>
        /// Check if service is installed and check if service installed and its current status
        /// </summary>
        /// <param name="sServiceName"></param>
        /// <param name="sStatus"></param>
        /// <param name="isCheckStatus"></param>
        /// <returns></returns>
        public static bool IsServiceInstalled(string sServiceName, string sStatus = "", bool isCheckStatus = false)
        {
            List<ServiceInformation> listServices = GetAllServicesInfo();
            foreach (ServiceInformation item in listServices)
            {
                if(isCheckStatus)
                {
                    if (item.name == sServiceName && item.status == sStatus)
                    {
                        return true;
                    }
                }
                else
                {
                    if (item.name == sServiceName)
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    }
}
