using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;
using System.Windows.Forms;

namespace ItemMaker
{
    class Weapon: Equippable
    {
        protected float m_fRange;
        protected float m_fDamage;
        protected float m_fRecovery;
        protected int m_iDamageType;

        public override void Save(BinaryWriter bw)
        {
            base.Save(bw);

            bw.Write(m_fRange);
            bw.Write(m_fDamage);
            bw.Write(m_fRecovery);
            bw.Write(m_iDamageType);            
        }

        public override void Load(BinaryReader br)
        {
            base.Load(br);

            m_fRange = br.ReadSingle();
            m_fDamage = br.ReadSingle();
            m_fRecovery = br.ReadSingle();
            m_iDamageType = br.ReadInt32();
        }

        public override void SetData(ArrayList textBoxes)
        {
            base.SetData(textBoxes);
            
            m_fRange = float.Parse(((TextBox)textBoxes[19]).Text);
            m_fDamage = float.Parse(((TextBox)textBoxes[20]).Text);
            m_fRecovery = float.Parse(((TextBox)textBoxes[21]).Text);
            m_iDamageType = int.Parse(((TextBox)textBoxes[22]).Text);
        }

        public override void SetFields(ArrayList textBoxes)
        {
            base.SetFields(textBoxes);

            ((TextBox)textBoxes[19]).Text = m_fRange.ToString();
            ((TextBox)textBoxes[20]).Text = m_fDamage.ToString();
            ((TextBox)textBoxes[21]).Text = m_fRecovery.ToString();
            ((TextBox)textBoxes[22]).Text = m_iDamageType.ToString();

        }
    }
}
