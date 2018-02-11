/*
 *  Created by Justin R. Wilson on 2/19/2017.
 *  Copyright 2017 Justin R. Wilson. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_REPORTER_AUTOMAKE_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_REPORTER_AUTOMAKE_HPP_INCLUDED

// Don't #include any Catch headers here - we can assume they are already
// included before this header.
// This is not good practice in general but is necessary in this case so this
// file can be distributed as a single header that works with the main
// Catch single header.

namespace Catch 
{
	
    struct AppVeyorReporter : StreamingReporterBase<AppVeyorReporter> 
	{
        AppVeyorReporter( ReporterConfig const& _config )
          :   StreamingReporterBase( _config )
        {
			m_reporterPrefs.shouldRedirectStdOut = true;
		}

        ~AppVeyorReporter() override;

        static std::string getDescription() 
		{
            return "Reports test results to AppVeyor";
        }

        void assertionStarting( AssertionInfo const& ) override 
		{
		}

        bool assertionEnded( AssertionStats const& /*_assertionStats*/ ) override { return true; }


		void testCaseEnded(TestCaseStats const& _testCaseStats) override
		{
			StreamingReporterBase::testCaseEnded(_testCaseStats);
			stringstream str;
			str << "appveyor.exe AddTest ";
			str << "\"" << _testCaseStats.testInfo.name << "\"";
			str << " -Framework CatchReporter";
			string fileName = _testCaseStats.testInfo.lineInfo.file;
			fileName = fileName.substr(fileName.rfind('\\') + 1);
			str << " -FileName FluxStd.exe";
			if (_testCaseStats.totals.assertions.allPassed())
			{
				str << " -Outcome Passed";
			}
			else if (_testCaseStats.totals.assertions.allOk())
			{
				str << " -Outcome Inconclusive";
			}
			else
			{
				str << " -Outcome Failed";
			}
			str << " -StdOut \"" << _testCaseStats.stdOut << "\"";
			str << " -StdErr \"" << _testCaseStats.stdErr << "\"";

			string cmd = str.str();
			system(cmd.c_str());
		}
    };

#ifdef CATCH_IMPL
    AppVeyorReporter::~AppVeyorReporter() {}
#endif

    CATCH_REGISTER_REPORTER( "appveyor", AppVeyorReporter)

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_REPORTER_AUTOMAKE_HPP_INCLUDED
