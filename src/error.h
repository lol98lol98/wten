
namespace wten { 

enum ERROR_CODE {
	ERROR_CODE_UNKNOWN,
	ERROR_CODE_INVALID_PARAMETER,
	ERROR_CODE_INTERNAL_ERROR,
	ERROR_CODE_DXLIB_INTERNAL_ERROR,

	//File関係
	ERROR_CODE_FILE_NOT_FOUND,			/**< ファイルが見つからなかった。 */

	//Window関係
	ERROR_CODE_OUTSIDE_RANGE,			/**< 座標指定などで範囲外を指定した。 */

	//UISelector関係
	ERROR_CODE_SELECTOR_TEXT_NOT_FOUND,	/**< 指定した文字列は見つからなかった。 */
	ERROR_CODE_SELECTOR_OUTSIDE_RANGE,		/**< 指定したindexは範囲外です。 */
	ERROR_CODE_SELECTOR_EMPTY,			/**< 中身が空なので実行できなかった。 */

	//Action関係
	ERROR_CODE_MP_SHORTAGE,			/**< MPが足らなかった。 */

	//Item関係
	ERROR_CODE_ITEM_BROKEN,			/**< 壊れたアイテムです。 */
	ERROR_CODE_ITEM_EQUIPED,			/**< 装備されたアイテムです。 */
	ERROR_CODE_ITEM_NONE_EQUIPED,		/**< 装備されていないアイテムです。 */
	ERROR_CODE_ITEM_CURSE,			/**< 呪われたアイテムです。 */
	ERROR_CODE_ITEM_CERTAIN,			/**< 鑑定済みアイテムです。 */
	ERROR_CODE_ITEM_NONE_CURSE,			/**< 呪われていないアイテムです。 */
	ERROR_CODE_ITEM_DISPEL_FAILURE,		/**< 解呪に失敗した。 */
	ERROR_CODE_ITEM_COUNT_OVER,			/**< 所持限界数オーバー。 */
	ERROR_CODE_ITEM_NOT_FOUND,			/**< 指定アイテムが見つかりませんでした。 */

	//charcter関係
	ERROR_CODE_CHAR_LOST,			/**< キャラがロストしました。 */
	ERROR_CODE_CHAR_NOT_FOUND,			/**< 指定したキャラは見つからなかった。 */

	//Event関係
	ERROR_CODE_POP_WINDOW_INVALID_WINDOW,	/**< 最前面のWindow以外を対象にPopWindowのEventが発生した。 */

	//PT関係
	ERROR_CODE_PT_NOT_FOUND,			/**< 指定したPTは見つからなかった。 */
};

#pragma warning(push)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
class Error : boost::noncopyable {
public:
	virtual ~Error() { }
	virtual const wchar_t* ToString(void) const = 0;
	virtual ERROR_CODE GetErrorCode(void) const = 0;
	virtual void Abort(void) = 0;
};
#pragma warning(pop)

} // wten
