from pydbus import SystemBus
from gi.repository import GLib
import time
import logging
from systemd.journal import JournalHandler
from systemdlogging.toolbox import init_systemd_logging

def Connection_state(state):
 if state:
  global start
  start = time.perf_counter()
  logger.debug('You've connected')
 else:
  global end
  end = time.perf_counter()
  time_spent = end - start
  logger.debug ('No connection!')
  logger.debug('Total time: ', time_spent)

def Connection_change(state):
 logger.debug(state)
 if state == 70: // NM_DEVICE_STATE_IP_CONFIG
  Connection_state(TRUE)
 else:
  if state == 20: // NM_DEVICE_STATE_UNAVAILABLE
   Connection_state(FALSE)

init_systemd_logging()
logger = logging.getLogger('my_logger')
logger.setLevel(logging.DEBUG)
bus = SystemBus()
nm = bus.get("org.freedesktop.NetworkManager")
Connection_change(nm.state())
nm.onStateChanged = Connection_change

loop = GLib.MainLoop()
loop.run()
