#pragma once
#include <string>
#include <memory>

#include "arraydata.h"
#include "string_op.h"
#include "type.h"

namespace sqf
{
	class configdata : public arraydata
	{
	private:
		std::string mname;
		std::string mparentname;
		std::weak_ptr<configdata> mlogicparent;
		std::shared_ptr<value> mvalue;
		bool misnull;

		std::shared_ptr<sqf::value> parent_unsafe();
		std::shared_ptr<sqf::value> navigate_unsafe(std::string nextnode);
		std::shared_ptr<sqf::value> navigate_full_unsafe(std::string nextnode);
	public:
		configdata(std::string name) : mname(std::move(name)), misnull(true) {}
		configdata() : mname("bin\\config.bin"), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::string name) : mname(std::move(name)), mlogicparent(std::move(logicparent)), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::string name, std::string parentname) : mname(std::move(name)), mparentname(std::move(parentname)), mlogicparent(std::move(logicparent)), misnull(false) {}

		std::shared_ptr<sqf::value> cfgvalue() const { return mvalue; }
		void cfgvalue(std::shared_ptr<sqf::value> val) { mvalue = val; }
		std::string name() const { return mname; }

		std::string tosqf() const override;
		bool equals(std::shared_ptr<data> d) const override { return d.get() == this; }
		size_t logicalparentcount() const { if (auto sptr = mlogicparent.lock()) { return sptr->logicalparentcount() + 1; } return 0; }
		std::shared_ptr<sqf::value> parent() { auto val = parent_unsafe(); return val.get() ? val : configNull(); }
		std::shared_ptr<sqf::value> navigate(std::string nextnode) { auto val = navigate_full_unsafe(nextnode); return val.get() ? val : configNull(); }

		bool is_null() { return misnull; }
		bool haslogicparent() { return !mlogicparent.expired(); }
		std::shared_ptr<sqf::value> logicparent();

		void mergeinto(std::shared_ptr<configdata>);

		static std::shared_ptr<sqf::value> configFile();
		static std::shared_ptr<sqf::value> configNull();
	};
}
