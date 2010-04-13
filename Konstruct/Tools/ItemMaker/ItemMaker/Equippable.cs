using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.Windows.Forms;

namespace ItemMaker
{
    class Equippable: Item
    {
        protected int m_iStrBonus;
        protected int m_iAgiBonus;
        protected int m_iIntBonus;
        protected int m_iConstBonus;
        protected int m_iHealthBonus;
        protected int m_iMentalBonus;

        //Resitances
        protected int m_iCrushResBonus;
        protected int m_iSlashResBonus;
        protected int m_iPierceResBonus;
        protected int m_iMentalResBonus;
        protected int m_iHeatResBonus;
        protected int m_iColdResBonus;
        protected int m_iElectResBonus;
        protected int m_iWaterResBonus;
        protected int m_iAcidResBonus;
        protected int m_iViralResBonus;
        protected int m_iHolyResBonus;
        protected int m_iDeathResBonus;

        public override void Save(BinaryWriter bw)
        {
            base.Save(bw);

            bw.Write(m_iStrBonus);
            bw.Write(m_iAgiBonus);
            bw.Write(m_iIntBonus);
            bw.Write(m_iConstBonus);
            bw.Write(m_iHealthBonus);
            bw.Write(m_iMentalBonus);

            bw.Write(m_iCrushResBonus);
            bw.Write(m_iSlashResBonus);
            bw.Write(m_iPierceResBonus);
            bw.Write(m_iMentalBonus);
            bw.Write(m_iHealthBonus);
            bw.Write(m_iColdResBonus);
            bw.Write(m_iElectResBonus);
            bw.Write(m_iWaterResBonus);
            bw.Write(m_iAcidResBonus);
            bw.Write(m_iViralResBonus);
            bw.Write(m_iHolyResBonus);
            bw.Write(m_iDeathResBonus);
        }

        public override void Load(BinaryReader br)
        {
            base.Load(br);

            m_iStrBonus = br.ReadInt32();
            m_iAgiBonus = br.ReadInt32();
            m_iIntBonus = br.ReadInt32();
            m_iConstBonus = br.ReadInt32();
            m_iHealthBonus = br.ReadInt32();
            m_iMentalBonus = br.ReadInt32();

            m_iCrushResBonus = br.ReadInt32();
            m_iSlashResBonus = br.ReadInt32();
            m_iPierceResBonus = br.ReadInt32();
            m_iMentalBonus = br.ReadInt32();
            m_iHealthBonus = br.ReadInt32();
            m_iColdResBonus = br.ReadInt32();
            m_iElectResBonus = br.ReadInt32();
            m_iWaterResBonus = br.ReadInt32();
            m_iAcidResBonus = br.ReadInt32();
            m_iViralResBonus = br.ReadInt32();
            m_iHolyResBonus = br.ReadInt32();
            m_iDeathResBonus = br.ReadInt32();
        }

        public override void SetData(System.Collections.ArrayList textBoxes)
        {
            base.SetData(textBoxes);

            m_iStrBonus = int.Parse(((TextBox)textBoxes[1]).Text);
            m_iAgiBonus = int.Parse(((TextBox)textBoxes[2]).Text);
            m_iIntBonus = int.Parse(((TextBox)textBoxes[3]).Text);
            m_iConstBonus = int.Parse(((TextBox)textBoxes[4]).Text);
            m_iHealthBonus = int.Parse(((TextBox)textBoxes[5]).Text);
            m_iMentalBonus = int.Parse(((TextBox)textBoxes[6]).Text);

            m_iCrushResBonus = int.Parse(((TextBox)textBoxes[7]).Text);
            m_iSlashResBonus = int.Parse(((TextBox)textBoxes[8]).Text);
            m_iPierceResBonus = int.Parse(((TextBox)textBoxes[9]).Text);
            m_iMentalBonus = int.Parse(((TextBox)textBoxes[10]).Text);
            m_iHealthBonus = int.Parse(((TextBox)textBoxes[11]).Text);
            m_iColdResBonus = int.Parse(((TextBox)textBoxes[12]).Text);
            m_iElectResBonus = int.Parse(((TextBox)textBoxes[13]).Text);
            m_iWaterResBonus = int.Parse(((TextBox)textBoxes[14]).Text);
            m_iAcidResBonus = int.Parse(((TextBox)textBoxes[15]).Text);
            m_iViralResBonus = int.Parse(((TextBox)textBoxes[16]).Text);
            m_iHolyResBonus = int.Parse(((TextBox)textBoxes[17]).Text);
            m_iDeathResBonus = int.Parse(((TextBox)textBoxes[18]).Text);

        }

        public override void SetFields(ArrayList textBoxes)
        {
            base.SetFields(textBoxes);

            ((TextBox)textBoxes[1]).Text = m_iStrBonus.ToString();
            ((TextBox)textBoxes[2]).Text = m_iAgiBonus.ToString();
            ((TextBox)textBoxes[3]).Text = m_iIntBonus.ToString();
            ((TextBox)textBoxes[4]).Text =  m_iConstBonus.ToString();
            ((TextBox)textBoxes[5]).Text =  m_iHealthBonus.ToString();
            ((TextBox)textBoxes[6]).Text = m_iMentalBonus.ToString();

            ((TextBox)textBoxes[7]).Text = m_iCrushResBonus.ToString();
            ((TextBox)textBoxes[8]).Text = m_iSlashResBonus.ToString();
            ((TextBox)textBoxes[9]).Text =  m_iPierceResBonus.ToString();
            ((TextBox)textBoxes[10]).Text =  m_iMentalBonus.ToString(); 
            ((TextBox)textBoxes[11]).Text = m_iHealthBonus.ToString();
            ((TextBox)textBoxes[12]).Text = m_iColdResBonus.ToString();
            ((TextBox)textBoxes[13]).Text = m_iElectResBonus.ToString();
            ((TextBox)textBoxes[14]).Text = m_iWaterResBonus.ToString();
            ((TextBox)textBoxes[15]).Text = m_iAcidResBonus.ToString();
            ((TextBox)textBoxes[16]).Text = m_iViralResBonus.ToString();
            ((TextBox)textBoxes[17]).Text = m_iHolyResBonus.ToString();
            ((TextBox)textBoxes[18]).Text = m_iDeathResBonus.ToString();
            
        }
    }
}
