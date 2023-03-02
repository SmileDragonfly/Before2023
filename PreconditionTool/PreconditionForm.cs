using log4net;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using TMSLog;
using TMSPreconditionTool.Tool;
using TMSPreconditionTool.Config;
using System.IO;
using System.Threading;

namespace TMSPreconditionTool
{
    public partial class PreconditionForm : Form
    {
        // Panel checking controls
        private ListView listViewApps;
        private ListView listViewServices;
        private List<Label> label_List_Check;
        private Button button_Install;

        // Panel install controls
        private static Panel panel_Install;
        private static Label label_Install;
        private static List<Label> label_List_Install;
        private static ProgressBar progressbar_Install;

        System.Windows.Forms.Timer checkingTimer;
        System.Windows.Forms.Timer installTimer;
        private bool isCheckSuccess;
        private static List<PreconditionConfig> listNeedToInstall;
        private Thread threadInstall;

        private static readonly ILog _logger = LogUtils.GetLogger();
        private static Config.Config _config;

        // Delegate
        private delegate void UpdateLabelDelegate(Label label, Object obj);
        private delegate void UpdateProgressBarDelegate(int iCount, int iMax, bool isDone);
        private static void UpdateLabel(Label label, Object obj)
        {
            if(label.InvokeRequired)
            {
                label.Invoke(new UpdateLabelDelegate(UpdateLabel), new object[] { label, obj });
            }
            else
            {
                if(obj is string)
                {
                    label.Text = (string)obj;
                }
                else if(obj is Color)
                {
                    label.ForeColor = (Color)obj;
                }
                else if(obj is Font)
                {
                    label.Font = (Font)obj;
                }
            }
        }

        private static void UpdateProgressbar(int iCount, int iMax, bool isDone)
        {
            if (progressbar_Install.InvokeRequired)
            {
                progressbar_Install.Invoke(new UpdateProgressBarDelegate(UpdateProgressbar), new object[] { iCount , iMax, isDone });
            }
            else
            {
                if(isDone)
                {
                    progressbar_Install.Maximum = iMax;
                    progressbar_Install.Value = iMax;
                }
                else
                {
                    progressbar_Install.Maximum = iMax;
                    progressbar_Install.Value = (iCount > (iMax - 2)) ? (iMax - 2):iCount;
                }
            }
        }

        public PreconditionForm()
        {
            InitializeComponent();
            // Set full screen
            Width = Screen.PrimaryScreen.Bounds.Width;
            Height = Screen.PrimaryScreen.Bounds.Height;
            panel_checking.Width = Width;
            panel_checking.Height = Height;
            label_checking.Parent = panel_checking;

            // Disable resize and move
            // ControlBox = false;
            //MaximumSize = new Size(Width, Height);
            //MinimumSize = new Size(Width, Height);

            // Add labels to display requirements
            _config = new Config.Config(Application.StartupPath);
            label_List_Check = new List<Label>();
            for (int i = 0; i < _config.listPreconditions.Count; i++)
            {
                Label label = new Label();
                label.Name = "label_check_" + (i + 1);
                label.Location = new Point(label_checking.Location.X, label_checking.Location.Y + 50 + 50 * i);
                label.Text = (i + 1) + "." + _config.listPreconditions[i].displayName;
                label.Parent = panel_checking;
                label.AutoSize = true;
                label.Show();
                label_List_Check.Add(label);
            }

            // Add button
            button_Install = new Button();
            button_Install.Location = new Point(Width - 200, Height - 200);
            button_Install.Size = new Size(button_Install.Size.Width * 2, button_Install.Size.Height * 2);
            button_Install.Text = "Install";
            button_Install.Name = "button_install";
            button_Install.Parent = panel_checking;
            button_Install.Click += ButtonInstall_Click;

            // Get all apps and services
            DisplayAppsAndServices(RegistryAccess.GetAllAppInfo(), ServiceAccess.GetAllServicesInfo());

            isCheckSuccess = true;
            // Start timer
            checkingTimer = new System.Windows.Forms.Timer();
            checkingTimer.Interval = 1000;
            checkingTimer.Enabled = true;
            checkingTimer.Tick += CheckingTimer_Tick;
            checkingTimer.Start();
        }

        private void CheckingTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                DoCheck();
                checkingTimer.Stop();
                if(isCheckSuccess)
                {
                    button_Install.Enabled = false;
                }
            }
            catch (Exception ex)
            {
                _logger.Error($"Exception CheckingTimer_Tick", ex);
            }
        }

        private void DoCheck()
        {
            try
            {
                // Begin check
                int iIndex = 0;
                List<ApplicationInfomation> listApps = RegistryAccess.GetAllAppInfo();
                List<ServiceInformation> listServices = ServiceAccess.GetAllServicesInfo();
                foreach (PreconditionConfig item in _config.listPreconditions)
                {
                    item.isSatify = false;
                    if (item.typeCheck == TypeCheck.Service.ToString())
                    {
                        foreach (ServiceInformation it in listServices)
                        {
                            if(it.name == item.checkName)
                            {
                                label_List_Check[iIndex].Text += " ✓";
                                label_List_Check[iIndex].ForeColor = Color.Green;
                                item.isSatify = true;
                                break;
                            }
                        }
                        if (label_List_Check[iIndex].ForeColor != Color.Green)
                        {
                            label_List_Check[iIndex].Text += " ✕";
                            label_List_Check[iIndex].ForeColor = Color.Red;
                            item.isSatify = false;
                            isCheckSuccess = false;
                        }
                    }
                    else if (item.typeCheck == TypeCheck.Application.ToString())
                    {
                        if(RegistryAccess.IsAppInstalled(item.checkName, item.version, item.versionCondition))
                        {
                            label_List_Check[iIndex].Text += " ✓";
                            label_List_Check[iIndex].ForeColor = Color.Green;
                            item.isSatify = true;
                        }
                        else
                        {
                            label_List_Check[iIndex].Text += " ✕";
                            label_List_Check[iIndex].ForeColor = Color.Red;
                            item.isSatify = false;
                            isCheckSuccess = false;
                        }
                    }
                    else
                    {
                        if(item.executed == Executed.Yes.ToString())
                        {
                            label_List_Check[iIndex].Text += " ✓";
                            label_List_Check[iIndex].ForeColor = Color.Green;
                            item.isSatify = true;
                        }
                        else
                        {
                            label_List_Check[iIndex].Text += " ✕";
                            label_List_Check[iIndex].ForeColor = Color.Red;
                            item.isSatify = false;
                            isCheckSuccess = false;
                        }
                    }
                    iIndex++;
                }
                label_checking.Text = "Check done!";
            }
            catch (Exception ex)
            {
                _logger.Error(@"Exception StartChecking", ex);
            }
        }

        private void DisplayAppsAndServices(List<ApplicationInfomation> listApps, List<ServiceInformation> listServices)
        {
            // Get the screen size
            int nWidth = Size.Width;
            int nHeight = Size.Height;
            // Display information to list view apps
            var listAppsSorted = listApps.OrderBy(it => it.displayName);

            ColumnHeader appNo = new ColumnHeader();
            appNo.Text = "No";
            appNo.Width = 30;
            appNo.TextAlign = HorizontalAlignment.Center;

            listViewApps = new ListView();
            listViewApps.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) | System.Windows.Forms.AnchorStyles.Left)));
            listViewApps.AutoArrange = false;
            listViewApps.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            listViewApps.FullRowSelect = true;
            listViewApps.HideSelection = false;
            listViewApps.Location = new System.Drawing.Point(nWidth/3, label_List_Check[0].Location.Y);
            listViewApps.MultiSelect = false;
            listViewApps.Name = "listView_Apps";
            listViewApps.Size = new System.Drawing.Size(nWidth/3, nHeight - 350);
            listViewApps.TabIndex = 1;
            listViewApps.UseCompatibleStateImageBehavior = false;
            listViewApps.View = System.Windows.Forms.View.Details;
            listViewApps.Parent = panel_checking;
            listViewApps.KeyUp += ListViewApps_KeyUp; 

            listViewApps.Columns.Clear();
            listViewApps.Columns.Add(appNo);
            listViewApps.Columns.Add("Appplication").Width = (nWidth/3 - 30) * 2/3;
            listViewApps.Columns.Add("Version").Width = (nWidth / 3 - 30) / 3;
            listViewApps.Items.Clear();
            listViewApps.BeginUpdate();
            int nNo = 1;
            foreach (var item in listAppsSorted)
            {
                ListViewItem viewItem = new ListViewItem(nNo.ToString());
                viewItem.SubItems.Add(item.displayName);
                viewItem.SubItems.Add(item.displayVersion);
                listViewApps.Items.Add(viewItem);
                nNo++;
            }
            listViewApps.EndUpdate();
            //listViewApps.Show();

            // Display information to list view services
            var listServicesSorted = listServices.OrderBy(it => it.name);

            ColumnHeader serviceNo = new ColumnHeader();
            serviceNo.Text = "No";
            serviceNo.Width = 30;
            serviceNo.TextAlign = HorizontalAlignment.Center;

            listViewServices = new ListView();
            listViewServices.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) | System.Windows.Forms.AnchorStyles.Left)));
            listViewServices.AutoArrange = false;
            listViewServices.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            listViewServices.FullRowSelect = true;
            listViewServices.HideSelection = false;
            listViewServices.Location = new System.Drawing.Point(nWidth / 3 * 2 + 30, label_List_Check[0].Location.Y);
            listViewServices.MultiSelect = false;
            listViewServices.Name = "listView_Apps";
            listViewServices.Size = new System.Drawing.Size(nWidth / 3 - 60 , nHeight - 350);
            listViewServices.TabIndex = 1;
            listViewServices.UseCompatibleStateImageBehavior = false;
            listViewServices.View = System.Windows.Forms.View.Details;
            listViewServices.Parent = panel_checking;
            listViewServices.KeyUp += ListViewServices_KeyUp;

            listViewServices.Columns.Clear();
            listViewServices.Columns.Add(serviceNo);
            listViewServices.Columns.Add("Service").Width = (nWidth / 3 - 90) * 2 / 3;
            listViewServices.Columns.Add("Status").Width = (nWidth / 3 - 90) / 3;
            listViewServices.Items.Clear();
            listViewServices.BeginUpdate();
            nNo = 1;
            foreach (var item in listServicesSorted)
            {
                ListViewItem viewItem = new ListViewItem(nNo.ToString());
                viewItem.SubItems.Add(item.name);
                viewItem.SubItems.Add(item.status);
                listViewServices.Items.Add(viewItem);
                nNo++;
            }
            listViewServices.EndUpdate();
            //listViewServices.Show();
        }

        private void ListViewServices_KeyUp(object sender, KeyEventArgs e)
        {
            if (sender != listViewServices) return;
            if(listViewServices.SelectedItems.Count > 0)
            {
                if (e.Control && e.KeyCode == Keys.C)
                    CopySelectedValuesToClipboard(listViewServices);
                if (e.KeyCode == Keys.F5)
                    ReloadListViewServices();
            }
        }

        private void ReloadListViewServices()
        {
            List<ServiceInformation> listServices = ServiceAccess.GetAllServicesInfo();
            var listServicesSorted = listServices.OrderBy(it => it.name);
            listViewServices.Items.Clear();
            listViewServices.BeginUpdate();
            int nNo = 1;
            foreach (var item in listServicesSorted)
            {
                ListViewItem viewItem = new ListViewItem(nNo.ToString());
                viewItem.SubItems.Add(item.name);
                viewItem.SubItems.Add(item.status);
                listViewServices.Items.Add(viewItem);
                nNo++;
            }
            listViewServices.EndUpdate();
        }

        private void ListViewApps_KeyUp(object sender, KeyEventArgs e)
        {
            if (sender != listViewApps) return;
            if (listViewApps.SelectedItems.Count > 0)
            {
                if (e.Control && e.KeyCode == Keys.C)
                    CopySelectedValuesToClipboard(listViewApps);
                if (e.KeyCode == Keys.F5)
                    ReloadListViewApps();
            }
        }

        private void ReloadListViewApps()
        {
            List<ApplicationInfomation> listApps = RegistryAccess.GetAllAppInfo();
            var listAppsSorted = listApps.OrderBy(it => it.displayName);
            listViewApps.Items.Clear();
            listViewApps.BeginUpdate();
            int nNo = 1;
            foreach (var item in listAppsSorted)
            {
                ListViewItem viewItem = new ListViewItem(nNo.ToString());
                viewItem.SubItems.Add(item.displayName);
                viewItem.SubItems.Add(item.displayVersion);
                listViewApps.Items.Add(viewItem);
                nNo++;
            }
            listViewApps.EndUpdate();
        }

        private void CopySelectedValuesToClipboard(ListView viewSource)
        {
            var builder = new StringBuilder();
            foreach (ListViewItem item in viewSource.SelectedItems)
                builder.AppendLine(item.SubItems[1].Text + ":" + item.SubItems[2].Text);
            Clipboard.SetText(builder.ToString());
        }

        private void InitPanelInstall()
        {
            panel_Install = new Panel();
            panel_Install.BackColor = System.Drawing.SystemColors.Control;
            panel_Install.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            panel_Install.Controls.Add(label_Install);
            panel_Install.Location = new System.Drawing.Point(0, 0);
            panel_Install.Name = "panel_install";
            panel_Install.Size = new Size(Width, Height);
            panel_Install.TabIndex = 0;
            Controls.Add(panel_Install);

            label_Install = new Label();
            label_Install.Name = "label_install";
            label_Install.Location = new Point(label_checking.Location.X, label_checking.Location.Y);
            label_Install.Text = "Installing...";
            label_Install.AutoSize = true;
            panel_Install.Controls.Add(label_Install);

            progressbar_Install = new ProgressBar();
            progressbar_Install.Name = "progressbar_install";
            progressbar_Install.Location = new Point(label_Install.Location.X + Width / 2, label_Install.Location.Y);
            panel_Install.Controls.Add(progressbar_Install);
        }


        private void ButtonInstall_Click(object sender, EventArgs e)
        {
            panel_checking.Visible = false;
            InitPanelInstall();
            panel_Install.Show();
            UpdatePanelInstall();
            // Start timer
            installTimer = new System.Windows.Forms.Timer();
            installTimer.Interval = 1000;
            installTimer.Enabled = true;
            installTimer.Tick += InstallTimer_Tick; ;
            installTimer.Start();
        }

        private void InstallTimer_Tick(object sender, EventArgs e)
        {
            FormClosed += PreconditionForm_FormClosed;
            threadInstall = new Thread(DoInstall);
            threadInstall.Start();
            installTimer.Stop();
        }

        private void PreconditionForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            _logger.Info("Closed main windows");
            if(threadInstall.IsAlive)
            {
                _logger.Warn("Install thread is not completed, force to stop it");
                threadInstall.Abort();
            }
        }

        private void UpdatePanelInstall()
        {
            // Get and display app need to install
            label_List_Install = new List<Label>();
            listNeedToInstall = new List<PreconditionConfig>();
            int iLabelCounter = 0;
            foreach (PreconditionConfig item in _config.listPreconditions)
            {
                if (!item.isSatify)
                {
                    Label label = new Label();
                    label.Name = "label_install_" + (iLabelCounter + 1);
                    label.Location = new Point(label_Install.Location.X, label_checking.Location.Y + 50 + 50 * iLabelCounter);
                    label.Text = (iLabelCounter + 1) + "." + item.displayName;
                    label.Parent = panel_Install;
                    label.AutoSize = true;
                    label.Show();
                    label_List_Install.Add(label);
                    listNeedToInstall.Add(item);
                    iLabelCounter++;
                }
            }
        }

        private static void DoInstall()
        {
            // Do install and update ui
            int iLabelCounter = 0;
            foreach (PreconditionConfig item in listNeedToInstall)
            {
                bool isInstalled = false;
                // Update label
                string sFullPath = Path.GetFullPath(item.path);
                string sText = "Installing " + item.displayName + ":" + sFullPath;
                UpdateLabel(label_Install, sText);
                UpdateLabel(label_List_Install[iLabelCounter], new Font(label_List_Install[iLabelCounter].Font, FontStyle.Bold));

                // Run command
                string sCommand = "\"" + sFullPath + "\"" + " " + item.command;
                System.Diagnostics.Process process = new System.Diagnostics.Process();
                System.Diagnostics.ProcessStartInfo startInfo = new System.Diagnostics.ProcessStartInfo();
                startInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                startInfo.FileName = "cmd.exe";
                startInfo.Arguments = "/c " + sCommand;
                startInfo.WorkingDirectory = Path.GetDirectoryName(sFullPath);
                process.StartInfo = startInfo;
                process.Start();
                _logger.Info($"DoInstall: {sCommand}, Working Dir: {startInfo.WorkingDirectory}");

                // Check if command install successfully
                int iTimerCounter = 0;
                while (iTimerCounter < item.timeout)
                {
                    if (item.typeCheck == TypeCheck.Service.ToString())
                    {
                        if (!ServiceAccess.IsServiceInstalled(item.checkName))
                        {
                            iTimerCounter++;
                            _logger.Info($"Installing {sCommand}-Count {iTimerCounter}-Estimate {item.estimateTime}-Timeout {item.timeout}");
                            Thread.Sleep(1000);
                            UpdateProgressbar(iTimerCounter, item.estimateTime, false);
                        }
                        else
                        {
                            _logger.Info($"Done install {sCommand}-Count {iTimerCounter}-Estimate {item.estimateTime}-Timeout {item.timeout}");
                            isInstalled = true;
                            UpdateProgressbar(iTimerCounter, item.estimateTime, true);
                        }
                    }
                    else if(item.typeCheck == TypeCheck.Application.ToString())
                    {
                        if (!RegistryAccess.IsAppInstalled(item.checkName, item.version, item.versionCondition))
                        {
                            iTimerCounter++;
                            _logger.Info($"Installing {sCommand}-Count {iTimerCounter}-Estimate {item.estimateTime}-Timeout {item.timeout}");
                            Thread.Sleep(1000);
                            UpdateProgressbar(iTimerCounter, item.estimateTime, false);
                        }
                        else
                        {
                            _logger.Info($"Done install {sCommand}-Count {iTimerCounter}-Estimate {item.estimateTime}-Timeout {item.timeout}");
                            isInstalled = true;
                            UpdateProgressbar(iTimerCounter, item.estimateTime, true);
                        }
                    }
                    else
                    {
                        if(iTimerCounter < item.estimateTime)
                        {
                            iTimerCounter++;
                            _logger.Info($"Installing {sCommand}-Count {iTimerCounter}-Estimate {item.estimateTime}-Timeout {item.timeout}");
                            Thread.Sleep(1000);
                            UpdateProgressbar(iTimerCounter, item.estimateTime, false);
                        }
                        else
                        {
                            _logger.Info($"Done install {sCommand}-Count {iTimerCounter}-Estimate {item.estimateTime}-Timeout {item.timeout}");
                            isInstalled = true;
                            UpdateProgressbar(iTimerCounter, item.estimateTime, true);
                            _config.UpdateExecutedField(item.configTag, "Yes");
                        }
                    }
                    // Update UI if installed or timeout
                    if (isInstalled)
                    {
                        break;
                    }
                }

                string sLabelText;
                Color labelColor;
                if (isInstalled)
                {
                    sLabelText = label_List_Install[iLabelCounter].Text + " ✓";
                    labelColor =  Color.Green;
                }
                else
                {
                    sLabelText = label_List_Install[iLabelCounter].Text + " ✕";
                    labelColor = Color.Red;
                }
                UpdateLabel(label_List_Install[iLabelCounter], sLabelText);
                UpdateLabel(label_List_Install[iLabelCounter], labelColor);
                iLabelCounter++;
            }
        }


        /// This is we are overriding base WIN32 window procedure to prevent the form from being moved by the mouse as well as resized by the mouse double click. /// ///
        protected override void WndProc(ref Message m)
        {
            const int WM_SYSCOMMAND = 0x0112;
            const int SC_MOVE = 0xF010;
            const int WM_NCLBUTTONDBLCLK = 0x00A3; //double click on a title bar a.k.a. non-client area of the form

            switch (m.Msg)
            {
                case WM_SYSCOMMAND:             //preventing the form from being moved by the mouse.
                    int command = m.WParam.ToInt32() & 0xfff0;
                    if (command == SC_MOVE)
                        return;
                    break;
            }

            if (m.Msg == WM_NCLBUTTONDBLCLK)       //preventing the form being resized by the mouse double click on the title bar.
            {
                m.Result = IntPtr.Zero;
                return;
            }

            base.WndProc(ref m);
        }
    }
}
