#include "scriptmodel.hpp"
#include <algorithm>
#include <iterator>

#include <qabstractitemmodel.h>
#include <qcontainerfwd.h>
#include <qlist.h>
#include <qnamespace.h>
#include <qtmetamacros.h>
#include <qtversionchecks.h>
#include <qtypes.h>
#include <qvariant.h>

void ScriptModel::updateValuesUnique(const QVariantList& newValues) {
	this->hasActiveIterators = true;
	this->mValues.reserve(newValues.size());

	auto iter = this->mValues.begin();
	auto newIter = newValues.begin();

	while (true) {
		if (newIter == newValues.end()) {
			if (iter == this->mValues.end()) break;

			auto startIndex = static_cast<qint32>(newValues.length());
			auto endIndex = static_cast<qint32>(this->mValues.length() - 1);

			this->beginRemoveRows(QModelIndex(), startIndex, endIndex);
			this->mValues.erase(iter, this->mValues.end());
			this->endRemoveRows();

			break;
		} else if (iter == this->mValues.end()) {
			// Prior branch ensures length is at least 1.
			auto startIndex = static_cast<qint32>(this->mValues.length());
			auto endIndex = static_cast<qint32>(newValues.length() - 1);

			this->beginInsertRows(QModelIndex(), startIndex, endIndex);
			this->mValues.append(newValues.sliced(startIndex));
			this->endInsertRows();

			break;
		} else if (*newIter != *iter) {
			auto oldIter = std::find(iter, this->mValues.end(), *newIter);

			if (oldIter != this->mValues.end()) {
				if (std::find(newIter, newValues.end(), *iter) == newValues.end()) {
					// Remove any entries we would otherwise move around that aren't in the new list.
					auto startIter = iter;

					do {
						++iter;
					} while (iter != this->mValues.end()
					         && std::find(newIter, newValues.end(), *iter) == newValues.end());

					auto index = static_cast<qint32>(std::distance(this->mValues.begin(), iter));
					auto startIndex = static_cast<qint32>(std::distance(this->mValues.begin(), startIter));

					this->beginRemoveRows(QModelIndex(), startIndex, index - 1);
					iter = this->mValues.erase(startIter, iter);
					this->endRemoveRows();
				} else {
					// Advance iters to capture a whole move sequence as a single operation if possible.
					auto oldStartIter = oldIter;
					do {
						++oldIter;
						++newIter;
					} while (oldIter != this->mValues.end() && newIter != newValues.end()
					         && *oldIter == *newIter);

					auto index = static_cast<qint32>(std::distance(this->mValues.begin(), iter));
					auto oldStartIndex =
					    static_cast<qint32>(std::distance(this->mValues.begin(), oldStartIter));
					auto oldIndex = static_cast<qint32>(std::distance(this->mValues.begin(), oldIter));
					auto len = oldIndex - oldStartIndex;

					this->beginMoveRows(QModelIndex(), oldStartIndex, oldIndex - 1, QModelIndex(), index);

					// While it is possible to optimize this further, it is currently not worth the time.
					for (auto i = 0; i != len; i++) {
						this->mValues.move(oldStartIndex + i, index + i);
					}

					iter = this->mValues.begin() + (index + len);
					this->endMoveRows();
				}
			} else {
				auto startNewIter = newIter;

				do {
					newIter++;
				} while (newIter != newValues.end()
				         && std::find(iter, this->mValues.end(), *newIter) == this->mValues.end());

				auto index = static_cast<qint32>(std::distance(this->mValues.begin(), iter));
				auto newIndex = static_cast<qint32>(std::distance(newValues.begin(), newIter));
				auto startNewIndex = static_cast<qint32>(std::distance(newValues.begin(), startNewIter));
				auto len = newIndex - startNewIndex;

				this->beginInsertRows(QModelIndex(), index, index + len - 1);
#if QT_VERSION <= QT_VERSION_CHECK(6, 8, 0)
				this->mValues.resize(this->mValues.length() + len);
#else
				this->mValues.resizeForOverwrite(this->mValues.length() + len);
#endif
				iter = this->mValues.begin() + index; // invalidated
				std::move_backward(iter, this->mValues.end() - len, this->mValues.end());
				iter = std::copy(startNewIter, newIter, iter);
				this->endInsertRows();
			}
		} else {
			++iter;
			++newIter;
		}
	}

	this->hasActiveIterators = false;
}

void ScriptModel::setValues(const QVariantList& newValues) {
	if (newValues == this->mValues) return;
	this->updateValuesUnique(newValues);
	emit this->valuesChanged();
}

qint32 ScriptModel::rowCount(const QModelIndex& parent) const {
	if (parent != QModelIndex()) return 0;
	return static_cast<qint32>(this->mValues.length());
}

QVariant ScriptModel::data(const QModelIndex& index, qint32 role) const {
	if (role != Qt::UserRole) return QVariant();
	return this->mValues.at(index.row());
}

QHash<int, QByteArray> ScriptModel::roleNames() const { return {{Qt::UserRole, "modelData"}}; }
