/*--------------------------------------------------------------------
pqChecker, shared library plug-in for OpenLDAP server / ppolicy overlay
Checking of password quality.
Copyright (C) 2014, Abdelhamid MEDDEB (abdelhamid@meddeb.net)  

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------*/

#include <ctype.h>
#include <syslog.h>
#include <stdbool.h>
#include <portable.h>
#include <slap.h>
#include <libintl.h>
#include <locale.h>

#include <pparamio.h>
#include <pqcheck.h>
#include <pqchecker.h>

int 
check_password(char *pPasswd, char **ppErrStr, Entry *e);

int 
check_password(char *pPasswd, char **ppErrStr, Entry *e)
{
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  openlog(PACKAGE, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL4);
  syslog(LOG_INFO, _("Checking password quality for %s."), e->e_name.bv_val);
  int rslt = LDAP_OPERATIONS_ERROR;
  char strParams[PARAMS_DATA_MAXLEN+1]; 
  if (readParams(strParams)) 
  {
    syslog(LOG_DEBUG, _("The quality parameters used: %s"),strParams);
    pp_params_t params = getParams(strParams, PARAMS_STORAGE_FORMAT);
    if ((params.upperMin > -1) && (params.lowerMin > -1) &&
        (params.digitMin > -1) && (params.specialMin > -1))
    {  
      pp_status_t pwdStatus = getStatus(pPasswd, params.forbiddens);
      if ((pwdStatus.upperNbr < params.upperMin) ||
          (pwdStatus.lowerNbr < params.lowerMin) ||
          (pwdStatus.digitNbr < params.digitMin) ||
          (pwdStatus.specialNbr < params.specialMin) ||
          (pwdStatus.forbiddenNbr > 0))
      {
        *ppErrStr = strdup(_("The password does not pass quality check."));
        syslog(LOG_INFO, _("Password rejected."));
      } else {
        rslt = LDAP_SUCCESS;
        syslog(LOG_INFO, _("Password accepted."));
      }
    } else {
      *ppErrStr = strdup(_("Unable to verify the quality of the password. Problem with parameters."));
      rslt = LDAP_OPERATIONS_ERROR;
      syslog(LOG_DEBUG, _("The passwords quality parameter is not operable."));
    }

  } else {
    *ppErrStr = strdup(_("Unable to verify the quality of the password. Problem in parameters."));
    rslt = LDAP_OPERATIONS_ERROR;
    syslog(LOG_DEBUG, _("The passwords quality parameter is not found."));
  }
  closelog();
	return (rslt);
}
