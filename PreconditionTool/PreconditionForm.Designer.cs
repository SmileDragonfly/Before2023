
namespace TMSPreconditionTool
{
    partial class PreconditionForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel_checking = new System.Windows.Forms.Panel();
            this.label_checking = new System.Windows.Forms.Label();
            this.panel_checking.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel_checking
            // 
            this.panel_checking.BackColor = System.Drawing.SystemColors.Control;
            this.panel_checking.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel_checking.Controls.Add(this.label_checking);
            this.panel_checking.Location = new System.Drawing.Point(0, 0);
            this.panel_checking.Name = "panel_checking";
            this.panel_checking.Size = new System.Drawing.Size(799, 450);
            this.panel_checking.TabIndex = 0;
            // 
            // label_checking
            // 
            this.label_checking.AutoSize = true;
            this.label_checking.Location = new System.Drawing.Point(41, 25);
            this.label_checking.Name = "label_checking";
            this.label_checking.Size = new System.Drawing.Size(61, 13);
            this.label_checking.TabIndex = 0;
            this.label_checking.Text = "Checking...";
            // 
            // PreconditionForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.panel_checking);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "PreconditionForm";
            this.Text = "TMSPreconditionTool";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.panel_checking.ResumeLayout(false);
            this.panel_checking.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel_checking;
        private System.Windows.Forms.Label label_checking;
    }
}

