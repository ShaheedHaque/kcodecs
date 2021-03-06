/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Universal charset detector code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *          Shy Shalom <shooshX@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef nsSBCSGroupProber_h__
#define nsSBCSGroupProber_h__

#include "nsCharSetProber.h"

#define NUM_OF_SBCS_PROBERS    14

namespace kencodingprober
{
class KCODECS_NO_EXPORT nsSBCSGroupProber: public nsCharSetProber
{
public:
    nsSBCSGroupProber();
    virtual ~nsSBCSGroupProber();
    nsProbingState HandleData(const char *aBuf, unsigned int aLen) Q_DECL_OVERRIDE;
    const char *GetCharSetName() Q_DECL_OVERRIDE;
    nsProbingState GetState(void) Q_DECL_OVERRIDE
    {
        return mState;
    };
    void      Reset(void) Q_DECL_OVERRIDE;
    float     GetConfidence(void) Q_DECL_OVERRIDE;
    void      SetOpion() Q_DECL_OVERRIDE {};

#ifdef DEBUG_PROBE
    void  DumpStatus() Q_DECL_OVERRIDE;
#endif

protected:
    nsProbingState mState;
    nsCharSetProber *mProbers[NUM_OF_SBCS_PROBERS];
    bool          mIsActive[NUM_OF_SBCS_PROBERS];
    int mBestGuess;
    unsigned int mActiveNum;
};
}

#endif /* nsSBCSGroupProber_h__ */

