#include "../framework/debug.h"
#include "../framework/log.h"

#include "processmanager.h"
#include "gameprocess.h"
#include "processinfo.h"
#include "../states/gamestate.h"

#define LOGCAT_PROCESSMANAGER "PROCESSMANAGER"

ProcessManager::ProcessManager(GameState *gameState)
{
	ASSERT(gameState != NULL);
	m_gameState = gameState;
}

ProcessManager::~ProcessManager()
{
	LOG_INFO(LOGCAT_PROCESSMANAGER, "ProcessManager shutting down.\n");

	while (!m_processes.empty())
	{
		ProcessInfo *processInfo = m_processes.back();
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Removing process %s as part of ProcessManager shutdown.\n", processInfo->GetDescriptor().c_str());
		processInfo->process->OnRemove();
		SAFE_DELETE(processInfo->process);
		SAFE_DELETE(processInfo);
		m_processes.pop_back();
	}

	// the queue likely will not have anything in it, but just in case...
	while (!m_queue.empty())
	{
		ProcessInfo *processInfo = m_queue.front();
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Removing queued process %s as part of ProcessManager shutdown.\n", processInfo->GetDescriptor().c_str());
		SAFE_DELETE(processInfo->process);
		SAFE_DELETE(processInfo);
		m_queue.pop_front();
	}
}

void ProcessManager::Remove(const stl::string &name)
{
	ProcessInfoList::iterator itor = GetProcessItorFor(name);
	StartTransitionOut(itor, true);
}

bool ProcessManager::HasProcess(const stl::string &name) const
{
	for (ProcessInfoList::const_iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (processInfo->name.length() > 0 && name == processInfo->name)
			return true;
	}
	
	return false;
}

void ProcessManager::OnPause(bool dueToOverlay)
{
	if (IsEmpty())
		return;

	if (dueToOverlay)
	{
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Pausing all active processes due to state being overlayed on the parent state.\n");
		for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
		{
			ProcessInfo *processInfo = *i;
			if (!processInfo->isInactive)
			{
				LOG_INFO(LOGCAT_PROCESSMANAGER, "Pausing process %s due to parent state overlay.\n", processInfo->GetDescriptor().c_str());
				processInfo->process->OnPause(true);
			}
		}
	}
	else
	{
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Transitioning out all active processes pending pause.\n");
		for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
		{
			ProcessInfo *processInfo = *i;
			if (!processInfo->isInactive)
				StartTransitionOut(i, false);
		}
	}
}

void ProcessManager::OnResume(bool fromOverlay)
{
	if (IsEmpty())
		return;

	if (fromOverlay)
	{
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Resuming all active processes due to overlay state being removed from overtop of parent state.\n");
		for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
		{
			ProcessInfo *processInfo = *i;
			if (!processInfo->isInactive)
			{
				LOG_INFO(LOGCAT_PROCESSMANAGER, "Resuming process %s due to overlay state removal.\n", processInfo->GetDescriptor().c_str());
				processInfo->process->OnResume(true);
			}
		}
	}
	else
	{
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Resuming processes.\n");
		for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
		{
			ProcessInfo *processInfo = *i;
			if (processInfo->isInactive && !processInfo->isBeingRemoved)
			{
				LOG_INFO(LOGCAT_PROCESSMANAGER, "Resuming process %s.\n", processInfo->GetDescriptor().c_str());
				processInfo->process->OnResume(false);

				processInfo->isInactive = false;
				processInfo->isTransitioning = true;
				processInfo->isTransitioningOut = false;
				processInfo->isTransitionStarting = true;
				LOG_INFO(LOGCAT_PROCESSMANAGER, "Transition into process %s started.\n", processInfo->GetDescriptor().c_str());
			}
		}
	}
}

void ProcessManager::OnAppGainFocus()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnAppGainFocus();
	}
}

void ProcessManager::OnAppLostFocus()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnAppLostFocus();
	}
}

void ProcessManager::OnAppPause()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnAppPause();
	}
}

void ProcessManager::OnAppResume()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnAppResume();
	}
}

void ProcessManager::OnLostContext()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnLostContext();
	}
}

void ProcessManager::OnNewContext()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnNewContext();
	}
}

void ProcessManager::OnRender(RenderContext *renderContext)
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnRender(renderContext);
	}
}

void ProcessManager::OnResize()
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnResize();
	}
}

void ProcessManager::OnUpdate(float delta)
{
	CleanupInactiveProcesses();
	CheckForFinishedProcesses();
	ProcessQueue();
	UpdateTransitions(delta);

	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive)
			processInfo->process->OnUpdate(delta);
	}
}

void ProcessManager::CleanupInactiveProcesses()
{
	if (m_processes.empty())
		return;

	ProcessInfoList::iterator i = m_processes.begin();
	while (i != m_processes.end())
	{
		ProcessInfo *processInfo = *i;
		if (processInfo->isInactive && processInfo->isBeingRemoved)
		{
			m_processes.erase(i++);

			LOG_INFO(LOGCAT_PROCESSMANAGER, "Deleting inactive process %s.\n", processInfo->GetDescriptor().c_str());
			SAFE_DELETE(processInfo->process);
			SAFE_DELETE(processInfo);
		}
		else
			++i;
	}
}

void ProcessManager::CheckForFinishedProcesses()
{
	if (m_processes.empty())
		return;

	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isInactive && processInfo->process->IsFinished() && !processInfo->isTransitioning)
		{
			LOG_INFO(LOGCAT_PROCESSMANAGER, "Process %s marked as finished.\n", processInfo->GetDescriptor().c_str());
			processInfo->isTransitioning = true;
			processInfo->isTransitioningOut = true;
			processInfo->isTransitionStarting = true;
			processInfo->isBeingRemoved = true;
			LOG_INFO(LOGCAT_PROCESSMANAGER, "Transition out of process %s started.\n", processInfo->GetDescriptor().c_str());
		}
	}
}

void ProcessManager::ProcessQueue()
{
	while (!m_queue.empty())
	{
		ProcessInfo *processInfo = m_queue.front();
		processInfo->isTransitioning = true;
		processInfo->isTransitioningOut = false;
		processInfo->isTransitionStarting = true;

		LOG_INFO(LOGCAT_PROCESSMANAGER, "Adding process %s from queue.\n", processInfo->GetDescriptor().c_str());
		processInfo->process->OnAdd();
		LOG_INFO(LOGCAT_PROCESSMANAGER, "Transition into process %s started.\n", processInfo->GetDescriptor().c_str());
		m_processes.push_back(processInfo);

		m_queue.pop_front();
	}
}

void ProcessManager::UpdateTransitions(float delta)
{
	if (m_processes.empty())
		return;

	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (processInfo->isTransitioning)
		{
			bool isDone = processInfo->process->OnTransition(delta, processInfo->isTransitioningOut, processInfo->isTransitionStarting);
			if (isDone)
			{
				LOG_INFO(LOGCAT_PROCESSMANAGER, "Transition %s process %s finished.\n", (processInfo->isTransitioningOut ? "out of" : "into"), processInfo->GetDescriptor().c_str());

				// if the process was being transitioned out, then we should mark
				// it as inactive, and trigger it's OnRemove event now
				if (processInfo->isTransitioningOut)
				{
					if (processInfo->isBeingRemoved)
					{
						LOG_INFO(LOGCAT_PROCESSMANAGER, "Removing process %s.\n", processInfo->GetDescriptor().c_str());
						processInfo->process->OnRemove();
					}
					else
					{
						LOG_INFO(LOGCAT_PROCESSMANAGER, "Pausing process %s.\n", processInfo->GetDescriptor().c_str());
						processInfo->process->OnPause(false);
					}
					processInfo->isInactive = true;
				}

				// done transitioning
				processInfo->isTransitioning = false;
				processInfo->isTransitioningOut = false;
			}
			processInfo->isTransitionStarting = false;
		}
	}
}

void ProcessManager::RemoveAll()
{
	LOG_INFO(LOGCAT_PROCESSMANAGER, "Transitioning out all processes pending removal.\n");
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (!processInfo->isTransitioning && !processInfo->isInactive)
			StartTransitionOut(i, true);
	}
}

void ProcessManager::Queue(ProcessInfo *newProcessInfo)
{
	ASSERT(newProcessInfo != NULL);
	ASSERT(newProcessInfo->process != NULL);

	LOG_INFO(LOGCAT_PROCESSMANAGER, "Queueing process %s.\n", newProcessInfo->GetDescriptor().c_str());
	m_queue.push_back(newProcessInfo);
}

void ProcessManager::StartTransitionOut(ProcessInfoList::iterator itor, bool forRemoval)
{
	ASSERT(itor != m_processes.end());
	ProcessInfo *processInfo = *itor;
	ASSERT(processInfo->isInactive == false);
	ASSERT(processInfo->isTransitioning == false);
	processInfo->isTransitioning = true;
	processInfo->isTransitioningOut = true;
	processInfo->isTransitionStarting = true;
	processInfo->isBeingRemoved = forRemoval;
	LOG_INFO(LOGCAT_PROCESSMANAGER, "Transition out of process %s started pending %s.\n", processInfo->GetDescriptor().c_str(), (forRemoval ? "removal" : "pause"));
}

bool ProcessManager::IsTransitioning() const
{
	for (ProcessInfoList::const_iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		const ProcessInfo *processInfo = *i;
		if (processInfo->isTransitioning)
			return true;
	}

	return false;
}

ProcessInfoList::iterator ProcessManager::GetProcessItorFor(const stl::string &name)
{
	ASSERT(name.length() > 0);
	if (name.length() == 0)
		return m_processes.end();
	
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (processInfo->name == name)
			return i;
	}
	
	return m_processes.end();
}

ProcessInfoList::iterator ProcessManager::GetProcessItorForFirstOf(GAMEPROCESS_TYPE processType)
{
	for (ProcessInfoList::iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (processInfo->process->GetTypeOf() == processType)
			return i;
	}

	return m_processes.end();
}

ProcessInfo* ProcessManager::GetProcessInfoFor(const GameProcess *process) const
{
	ASSERT(process != NULL);
	for (ProcessInfoList::const_iterator i = m_processes.begin(); i != m_processes.end(); ++i)
	{
		ProcessInfo *processInfo = *i;
		if (processInfo->process == process)
			return processInfo;
	}

	return NULL;
}

